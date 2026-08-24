#include "edit_tailscale.h"
#include "ui_edit_tailscale.h"

#include "fmt/WgTailscaleBean.hpp"

EditTailscale::EditTailscale(QWidget *parent) : QWidget(parent), ui(new Ui::EditTailscale) {
    ui->setupUi(this);
}

EditTailscale::~EditTailscale() {
    delete ui;
}

void EditTailscale::onStart(std::shared_ptr<NekoGui::ProxyEntity> _ent) {
    this->ent = _ent;
    auto bean = this->ent->TailscaleBean();

    P_LOAD_STRING(auth_key);
    P_LOAD_STRING(hostname);
    P_LOAD_STRING(control_url);
    P_LOAD_BOOL(accept_routes);
    P_LOAD_BOOL(ephemeral);
}

bool EditTailscale::onEnd() {
    auto bean = this->ent->TailscaleBean();

    P_SAVE_STRING(auth_key);
    P_SAVE_STRING(hostname);
    P_SAVE_STRING(control_url);
    P_SAVE_BOOL(accept_routes);
    P_SAVE_BOOL(ephemeral);
    return true;
}
