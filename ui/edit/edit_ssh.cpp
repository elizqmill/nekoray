#include "edit_ssh.h"
#include "ui_edit_ssh.h"

#include "fmt/SshBean.hpp"

EditSSH::EditSSH(QWidget *parent) : QWidget(parent), ui(new Ui::EditSSH) {
    ui->setupUi(this);
}

EditSSH::~EditSSH() {
    delete ui;
}

void EditSSH::onStart(std::shared_ptr<NekoGui::ProxyEntity> _ent) {
    this->ent = _ent;
    auto bean = this->ent->SshBean();

    P_LOAD_STRING(user);
    P_LOAD_STRING(password);
    P_LOAD_STRING_PLAIN(privateKey);
    P_LOAD_STRING_PLAIN(hostKey);
    ui->private_key_passphrase->setText(bean->privateKeyPassphrase);
}

bool EditSSH::onEnd() {
    auto bean = this->ent->SshBean();

    P_SAVE_STRING(user);
    P_SAVE_STRING(password);
    P_SAVE_STRING_PLAIN(privateKey);
    P_SAVE_STRING_PLAIN(hostKey);
    bean->privateKeyPassphrase = ui->private_key_passphrase->text();
    return true;
}
