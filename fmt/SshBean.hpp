#pragma once

#include "fmt/AbstractBean.hpp"

namespace NekoGui_fmt {
    class SshBean : public AbstractBean {
    public:
        QString user = "root";
        QString password = "";
        QString privateKey = "";
        QString privateKeyPassphrase = "";
        QString hostKey = "";

        explicit SshBean() : AbstractBean(0) {
            _add(new configItem("user", &user, itemType::string));
            _add(new configItem("password", &password, itemType::string));
            _add(new configItem("privateKey", &privateKey, itemType::string));
            _add(new configItem("privateKeyPassphrase", &privateKeyPassphrase, itemType::string));
            _add(new configItem("hostKey", &hostKey, itemType::string));
        };

        QString DisplayType() override { return "SSH"; };

        CoreObjOutboundBuildResult BuildCoreObjSingBox() override;
    };
} // namespace NekoGui_fmt
