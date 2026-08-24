#include "edit_anytls.h"
#include "ui_edit_anytls.h"

#include "fmt/AnyTLSBean.hpp"

EditAnyTLS::EditAnyTLS(QWidget *parent) : QWidget(parent), ui(new Ui::EditAnyTLS) {
    ui->setupUi(this);
}

EditAnyTLS::~EditAnyTLS() {
    delete ui;
}

void EditAnyTLS::onStart(std::shared_ptr<NekoGui::ProxyEntity> _ent) {
    this->ent = _ent;
    auto bean = this->ent->AnyTLSBean();

    P_LOAD_STRING(password);
    P_LOAD_STRING(sni);
    P_LOAD_STRING(alpn);
    P_LOAD_BOOL(allowInsecure);
    P_LOAD_BOOL(disableSni);
    ui->ca_text->setPlainText(bean->caText);
}

bool EditAnyTLS::onEnd() {
    auto bean = this->ent->AnyTLSBean();

    P_SAVE_STRING(password);
    P_SAVE_STRING(sni);
    P_SAVE_STRING(alpn);
    P_SAVE_BOOL(allowInsecure);
    P_SAVE_BOOL(disableSni);
    bean->caText = ui->ca_text->toPlainText();
    return true;
}
