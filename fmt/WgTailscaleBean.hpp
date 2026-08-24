#pragma once

#include "fmt/AbstractBean.hpp"

namespace NekoGui_fmt {
    // sing-box 1.11+ WireGuard endpoint
    class WgBean : public AbstractBean {
    public:
        QString private_key = "";
        QString peer_public_key = "";
        QString pre_shared_key = "";
        QString local_address = "172.16.0.2/32";
        QList<int> reserved = {};
        int mtu = 0;

        explicit WgBean() : AbstractBean(0) {
            _add(new configItem("private_key", &private_key, itemType::string));
            _add(new configItem("peer_public_key", &peer_public_key, itemType::string));
            _add(new configItem("pre_shared_key", &pre_shared_key, itemType::string));
            _add(new configItem("local_address", &local_address, itemType::string));
            _add(new configItem("reserved", &reserved, itemType::integerList));
            _add(new configItem("mtu", &mtu, itemType::integer));
        };

        QString DisplayType() override { return "WireGuard"; };

        QJsonObject BuildEndpoint(const QString &tag);
    };

    // sing-box 1.12 Tailscale endpoint
    class TailscaleBean : public AbstractBean {
    public:
        QString auth_key = "";
        QString hostname = "";
        QString control_url = "";
        bool accept_routes = false;
        bool ephemeral = false;

        explicit TailscaleBean() : AbstractBean(0) {
            _add(new configItem("auth_key", &auth_key, itemType::string));
            _add(new configItem("hostname", &hostname, itemType::string));
            _add(new configItem("control_url", &control_url, itemType::string));
            _add(new configItem("accept_routes", &accept_routes, itemType::boolean));
            _add(new configItem("ephemeral", &ephemeral, itemType::boolean));
        };

        QString DisplayType() override { return "Tailscale"; };

        QJsonObject BuildEndpoint(const QString &tag);
    };
} // namespace NekoGui_fmt
