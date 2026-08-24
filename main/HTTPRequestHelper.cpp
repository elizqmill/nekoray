#include "HTTPRequestHelper.hpp"

#include <QByteArray>
#include <QNetworkProxy>
#include <QEventLoop>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMetaEnum>
#include <QTimer>

#include "main/NekoGui.hpp"

namespace NekoGui_network {

    // Parse a JSON object of HTTP request headers, e.g.
    // {"X-HWID": "b4d9f2a1c8e37605", "X-Device-OS": "Android"}
    static QList<QPair<QByteArray, QByteArray>> ParseJsonHeaders(const QString &json, QString *error) {
        QList<QPair<QByteArray, QByteArray>> headers;
        if (json.trimmed().isEmpty()) return headers;

        QJsonParseError parseError;
        auto doc = QJsonDocument::fromJson(json.trimmed().toUtf8(), &parseError);
        if (parseError.error != QJsonParseError::NoError || !doc.isObject()) {
            *error = parseError.errorString();
            return headers;
        }

        const auto object = doc.object();
        for (auto it = object.constBegin(); it != object.constEnd(); ++it) {
            auto key = it.key().trimmed();
            if (key.isEmpty()) continue;

            QString value;
            switch (it.value().type()) {
                case QJsonValue::String:
                    value = it.value().toString();
                    break;
                case QJsonValue::Double:
                    value = QString::number(it.value().toDouble());
                    break;
                case QJsonValue::Bool:
                    value = it.value().toBool() ? QStringLiteral("true") : QStringLiteral("false");
                    break;
                case QJsonValue::Object:
                    value = QString::fromUtf8(QJsonDocument(it.value().toObject()).toJson(QJsonDocument::Compact));
                    break;
                case QJsonValue::Array:
                    value = QString::fromUtf8(QJsonDocument(it.value().toArray()).toJson(QJsonDocument::Compact));
                    break;
                default:
                    continue;
            }
            if (value.trimmed().isEmpty()) continue;

            headers.append({key.toUtf8(), value.toUtf8()});
        }
        return headers;
    }

    NekoHTTPResponse NetworkRequestHelper::HttpGet(const QUrl &url, const QString &customHeadersJson, const QString &customUserAgent) {
        QNetworkRequest request;
        QNetworkAccessManager accessManager;
        request.setUrl(url);
        // Set proxy
        if (NekoGui::dataStore->sub_use_proxy) {
            QNetworkProxy p;
            // Note: sing-box mixed socks5 protocol error
            p.setType(QNetworkProxy::HttpProxy);
            p.setHostName("127.0.0.1");
            p.setPort(NekoGui::dataStore->inbound_socks_port);
            if (NekoGui::dataStore->inbound_auth->NeedAuth()) {
                p.setUser(NekoGui::dataStore->inbound_auth->username);
                p.setPassword(NekoGui::dataStore->inbound_auth->password);
            }
            accessManager.setProxy(p);
            if (NekoGui::dataStore->started_id < 0) {
                return NekoHTTPResponse{QObject::tr("Request with proxy but no profile started.")};
            }
        }
        if (accessManager.proxy().type() == QNetworkProxy::Socks5Proxy) {
            auto cap = accessManager.proxy().capabilities();
            accessManager.proxy().setCapabilities(cap | QNetworkProxy::HostNameLookupCapability);
        }
        // Set attribute
#if (QT_VERSION >= QT_VERSION_CHECK(5, 9, 0))
        request.setAttribute(QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::NoLessSafeRedirectPolicy);
#endif
        // User-Agent: group override has priority over global setting
        auto userAgent = customUserAgent.trimmed().isEmpty() ? NekoGui::dataStore->GetUserAgent() : customUserAgent.trimmed();
        request.setHeader(QNetworkRequest::KnownHeaders::UserAgentHeader, userAgent);
        // Custom request headers (JSON)
        QString headersError;
        const auto customHeaders = ParseJsonHeaders(customHeadersJson, &headersError);
        if (!headersError.isEmpty()) {
            // A malformed headers blob must not kill the whole request - skip custom headers and fetch anyway.
            MW_show_log(QObject::tr("Invalid request headers JSON, ignoring: %1").arg(headersError));
        }
        for (const auto &[key, value]: customHeaders) {
            request.setRawHeader(key, value);
        }
        if (NekoGui::dataStore->sub_insecure) {
            QSslConfiguration c;
            c.setPeerVerifyMode(QSslSocket::PeerVerifyMode::VerifyNone);
            request.setSslConfiguration(c);
        }
        //
        auto _reply = accessManager.get(request);
        connect(_reply, &QNetworkReply::sslErrors, _reply, [](const QList<QSslError> &errors) {
            QStringList error_str;
            for (const auto &err: errors) {
                error_str << err.errorString();
            }
            MW_show_log(QStringLiteral("SSL Errors: %1 %2").arg(error_str.join(","), NekoGui::dataStore->sub_insecure ? "(Ignored)" : ""));
        });
        // Wait for response
        auto abortTimer = new QTimer;
        abortTimer->setSingleShot(true);
        abortTimer->setInterval(10000);
        QObject::connect(abortTimer, &QTimer::timeout, _reply, &QNetworkReply::abort);
        abortTimer->start();
        {
            QEventLoop loop;
            QObject::connect(_reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
            loop.exec();
        }
        if (abortTimer != nullptr) {
            abortTimer->stop();
            abortTimer->deleteLater();
        }
        //
        auto result = NekoHTTPResponse{_reply->error() == QNetworkReply::NetworkError::NoError ? "" : _reply->errorString(),
                                       _reply->readAll(), _reply->rawHeaderPairs()};
        _reply->deleteLater();
        return result;
    }

    QString NetworkRequestHelper::GetHeader(const QList<QPair<QByteArray, QByteArray>> &header, const QString &name) {
        for (const auto &p: header) {
            if (QString(p.first).toLower() == name.toLower()) return p.second;
        }
        return "";
    }

} // namespace NekoGui_network
