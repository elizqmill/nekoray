#include "dialog_edit_group.h"
#include "ui_dialog_edit_group.h"

#include "db/Database.hpp"
#include "ui/mainwindow_interface.h"

#include <QClipboard>
#include <QJsonDocument>
#include <QJsonParseError>

#define ADJUST_SIZE runOnUiThread([=] { adjustSize(); adjustPosition(mainwindow); }, this);

DialogEditGroup::DialogEditGroup(const std::shared_ptr<NekoGui::Group> &ent, QWidget *parent) : QDialog(parent), ui(new Ui::DialogEditGroup) {
    ui->setupUi(this);
    this->ent = ent;

    connect(ui->type, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, [=](int index) {
        ui->cat_sub->setHidden(index == 0);
        ADJUST_SIZE
    });

    ui->name->setText(ent->name);
    ui->archive->setChecked(ent->archive);
    ui->skip_auto_update->setChecked(ent->skip_auto_update);
    ui->url->setText(ent->url);
    ui->request_headers->setPlainText(ent->request_headers);
    ui->user_agent->setText(ent->user_agent);
    ui->user_agent->setPlaceholderText(tr("Default: %1").arg(NekoGui::dataStore->GetUserAgent(true)));
    ui->type->setCurrentIndex(ent->url.isEmpty() ? 0 : 1);
    ui->type->currentIndexChanged(ui->type->currentIndex());
    ui->manually_column_width->setChecked(ent->manually_column_width);
    ui->cat_share->setVisible(false);

    if (ent->id >= 0) { // already a group
        ui->type->setDisabled(true);
        if (!ent->Profiles().isEmpty()) {
            ui->cat_share->setVisible(true);
        }
    } else { // new group
        ui->front_proxy->hide();
        ui->front_proxy_l->hide();
        ui->front_proxy_clear->hide();
    }

    CACHE.front_proxy = ent->front_proxy_id;
    refresh_front_proxy();
    connect(ui->front_proxy_clear, &QPushButton::clicked, this, [=] {
        CACHE.front_proxy = -1;
        refresh_front_proxy();
    });

    connect(ui->copy_links, &QPushButton::clicked, this, [=] {
        QStringList links;
        for (const auto &[_, profile]: NekoGui::profileManager->profiles) {
            if (profile->gid != ent->id) continue;
            links += profile->bean->ToShareLink();
        }
        QApplication::clipboard()->setText(links.join("\n"));
        MessageBoxInfo(software_name, tr("Copied"));
    });
    connect(ui->copy_links_nkr, &QPushButton::clicked, this, [=] {
        QStringList links;
        for (const auto &[_, profile]: NekoGui::profileManager->profiles) {
            if (profile->gid != ent->id) continue;
            links += profile->bean->ToNekorayShareLink(profile->type);
        }
        QApplication::clipboard()->setText(links.join("\n"));
        MessageBoxInfo(software_name, tr("Copied"));
    });

    ADJUST_SIZE
}

DialogEditGroup::~DialogEditGroup() {
    delete ui;
}

void DialogEditGroup::accept() {
    auto request_headers_txt = ui->request_headers->toPlainText().trimmed();
    if (!request_headers_txt.isEmpty()) {
        QJsonParseError parseError;
        auto doc = QJsonDocument::fromJson(request_headers_txt.toUtf8(), &parseError);
        if (parseError.error != QJsonParseError::NoError || !doc.isObject()) {
            MessageBoxWarning(tr("Warning"), tr("Invalid JSON") + ": " + parseError.errorString());
            return;
        }
        // normalize
        ui->request_headers->setPlainText(QString::fromUtf8(QJsonDocument(doc.object()).toJson(QJsonDocument::Indented)));
    }
    if (ent->id >= 0) { // already a group
        if (!ent->url.isEmpty() && ui->url->text().isEmpty()) {
            MessageBoxWarning(tr("Warning"), tr("Please input URL"));
            return;
        }
    }
    ent->name = ui->name->text();
    ent->url = ui->url->text();
    ent->request_headers = request_headers_txt;
    ent->user_agent = ui->user_agent->text().trimmed();
    ent->archive = ui->archive->isChecked();
    ent->skip_auto_update = ui->skip_auto_update->isChecked();
    ent->manually_column_width = ui->manually_column_width->isChecked();
    ent->front_proxy_id = CACHE.front_proxy;
    QDialog::accept();
}

void DialogEditGroup::refresh_front_proxy() {
    auto fEnt = NekoGui::profileManager->GetProfile(CACHE.front_proxy);
    ui->front_proxy->setText(fEnt == nullptr ? tr("None") : fEnt->bean->DisplayTypeAndName());
}

void DialogEditGroup::on_front_proxy_clicked() {
    auto parent = dynamic_cast<QWidget *>(this->parent());
    parent->hide();
    this->hide();
    GetMainWindow()->start_select_mode(this, [=](int id) {
        CACHE.front_proxy = id;
        refresh_front_proxy();
        parent->show();
        show();
    });
}
