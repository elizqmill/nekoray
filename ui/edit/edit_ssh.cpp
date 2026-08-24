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
    P_LOAD_STRING(privateKeyPassphrase);
    ui->private_key->setPlainText(bean->privateKey.replace("\\n", "\n"));
    ui->host_key->setPlainText(bean->hostKey.replace("\\n", "\n"));
}

bool EditSSH::onEnd() {
    auto bean = this->ent->SshBean();

    P_SAVE_STRING(user);
    P_SAVE_STRING(password);
    P_SAVE_STRING(privateKeyPassphrase);
    bean->privateKey = ui->private_key->toPlainText();
    bean->hostKey = ui->host_key->toPlainText();
    return true;
}
