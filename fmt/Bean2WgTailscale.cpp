#include "db/ProxyEntity.hpp"
#include "fmt/includes.h"

namespace NekoGui_fmt {
    QJsonObject WgBean::BuildEndpoint(const QString &tag) {
        QJsonObject ep{
            {"type", "wireguard"},
            {"tag", tag},
            {"server", serverAddress},
            {"server_port", serverPort <= 0 ? 51820 : serverPort},
            {"local_address", QList2QJsonArray(local_address.split(","))},
            {"private_key", private_key},
            {"peer_public_key", peer_public_key},
        };
        if (!pre_shared_key.trimmed().isEmpty()) ep["pre_shared_key"] = pre_shared_key;
        if (!reserved.isEmpty()) {
            QJsonArray r;
            for (auto x: reserved) r += x;
            ep["reserved"] = r;
        }
        if (mtu > 0) ep["mtu"] = mtu;
        return ep;
    }

    QJsonObject TailscaleBean::BuildEndpoint(const QString &tag) {
        QJsonObject ep{
            {"type", "tailscale"},
            {"tag", tag},
        };
        if (!auth_key.trimmed().isEmpty()) ep["auth_key"] = auth_key;
        if (!hostname.trimmed().isEmpty()) ep["hostname"] = hostname;
        if (!control_url.trimmed().isEmpty()) ep["control_url"] = control_url;
        if (accept_routes) ep["accept_routes"] = true;
        if (ephemeral) ep["ephemeral"] = true;
        return ep;
    }
} // namespace NekoGui_fmt
