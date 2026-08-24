#include "edit_wg.h"
#include "ui_edit_wg.h"

#include "fmt/WgTailscaleBean.hpp"

EditWG::EditWG(QWidget *parent) : QWidget(parent), ui(new Ui::EditWG) {
    ui->setupUi(this);
}

EditWG::~EditWG() {
    delete ui;
}

void EditWG::onStart(std::shared_ptr<NekoGui::ProxyEntity> _ent) {
    this->ent = _ent;
    auto bean = this->ent->WgBean();

    P_LOAD_STRING(private_key);
    P_LOAD_STRING(peer_public_key);
    P_LOAD_STRING(pre_shared_key);
    P_LOAD_STRING(local_address);
    P_LOAD_INT(mtu);

    QString reserved;
    for (auto x: bean->reserved) reserved += (reserved.isEmpty() ? "" : ",") + Int2String(x);
    ui->reserved->setText(reserved);
}

bool EditWG::onEnd() {
    auto bean = this->ent->WgBean();

    P_SAVE_STRING(private_key);
    P_SAVE_STRING(peer_public_key);
    P_SAVE_STRING(pre_shared_key);
    P_SAVE_STRING(local_address);
    P_SAVE_INT(mtu);

    QList<int> reserved;
    for (const auto &x: ui->reserved->text().split(",")) {
        auto v = x.trimmed().toInt();
        if (v >= 0) reserved += v;
    }
    bean->reserved = reserved;
    return true;
}
