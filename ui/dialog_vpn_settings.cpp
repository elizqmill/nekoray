#include "dialog_vpn_settings.h"
#include "ui_dialog_vpn_settings.h"

#include "main/GuiUtils.hpp"
#include "main/NekoGui.hpp"
#include "ui/mainwindow_interface.h"

#include <QDialogButtonBox>
#include <QListWidget>
#include <QMessageBox>
#include <QProcess>

DialogVPNSettings::DialogVPNSettings(QWidget *parent) : QDialog(parent), ui(new Ui::DialogVPNSettings) {
    ui->setupUi(this);
    ADD_ASTERISK(this);

    ui->fake_dns->setChecked(NekoGui::dataStore->fake_dns);
    ui->vpn_implementation->setCurrentIndex(NekoGui::dataStore->vpn_implementation);
    ui->vpn_mtu->setCurrentText(Int2String(NekoGui::dataStore->vpn_mtu));
    ui->vpn_ipv6->setChecked(NekoGui::dataStore->vpn_ipv6);
    ui->hide_console->setChecked(NekoGui::dataStore->vpn_hide_console);
#ifndef Q_OS_WIN
    ui->hide_console->setVisible(false);
#endif
    ui->strict_route->setChecked(NekoGui::dataStore->vpn_strict_route);
    ui->single_core->setChecked(NekoGui::dataStore->vpn_internal_tun);
    //
    D_LOAD_STRING_PLAIN(vpn_rule_cidr)
    D_LOAD_STRING_PLAIN(vpn_rule_process)
    //
    connect(ui->select_processes, &QPushButton::clicked, this, [=] {
        // list running processes
        QProcess p;
#ifdef Q_OS_WIN
        p.start("tasklist", {"/fo", "csv", "/nh"});
#else
        p.start("ps", {"-eo", "comm="});
#endif
        p.waitForFinished(5000);
        auto output = QString::fromLocal8Bit(p.readAllStandardOutput());
        QStringList names;
        for (const auto &line: output.split("\n")) {
            auto name = line.trimmed();
#ifdef Q_OS_WIN
            if (name.startsWith("\"")) name = name.section("\",\"", 0, 0).remove(0, 1);
#endif
            if (name.isEmpty()) continue;
            if (!names.contains(name)) names << name;
        }
        names.sort(Qt::CaseInsensitive);
        if (names.isEmpty()) {
            MessageBoxWarning(tr("Warning"), tr("Could not list processes"));
            return;
        }
        // selection dialog
        QDialog dialog(this);
        dialog.setWindowTitle(tr("Select running processes"));
        auto layout = new QVBoxLayout(&dialog);
        auto list = new QListWidget(&dialog);
        for (const auto &name: names) {
            auto item = new QListWidgetItem(name, list);
            item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
            item->setCheckState(ui->vpn_rule_process->toPlainText().contains(name) ? Qt::Checked : Qt::Unchecked);
        }
        layout->addWidget(list);
        auto bb = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
        connect(bb, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
        connect(bb, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
        layout->addWidget(bb);
        if (dialog.exec() != QDialog::Accepted) return;
        // merge checked into the text edit
        QStringList current;
        for (const auto &l: ui->vpn_rule_process->toPlainText().split("\n")) {
            auto t = l.trimmed();
            if (!t.isEmpty()) current << t;
        }
        for (int i = 0; i < list->count(); i++) {
            auto item = list->item(i);
            if (item->checkState() != Qt::Checked) continue;
            if (!current.contains(item->text())) current << item->text();
        }
        current.sort(Qt::CaseInsensitive);
        ui->vpn_rule_process->setPlainText(current.join("\n"));
    });
    //
    connect(ui->whitelist_mode, &QCheckBox::stateChanged, this, [=](int state) {
        if (state == Qt::Checked) {
            ui->gb_cidr->setTitle(tr("Proxy CIDR"));
            ui->gb_process_name->setTitle(tr("Proxy Process Name"));
        } else {
            ui->gb_cidr->setTitle(tr("Bypass CIDR"));
            ui->gb_process_name->setTitle(tr("Bypass Process Name"));
        }
    });
    ui->whitelist_mode->setChecked(NekoGui::dataStore->vpn_rule_white);
}

DialogVPNSettings::~DialogVPNSettings() {
    delete ui;
}

void DialogVPNSettings::accept() {
    //
    auto mtu = ui->vpn_mtu->currentText().toInt();
    if (mtu > 10000 || mtu < 1000) mtu = 9000;
    NekoGui::dataStore->vpn_implementation = ui->vpn_implementation->currentIndex();
    NekoGui::dataStore->fake_dns = ui->fake_dns->isChecked();
    NekoGui::dataStore->vpn_mtu = mtu;
    NekoGui::dataStore->vpn_ipv6 = ui->vpn_ipv6->isChecked();
    NekoGui::dataStore->vpn_hide_console = ui->hide_console->isChecked();
    NekoGui::dataStore->vpn_strict_route = ui->strict_route->isChecked();
    NekoGui::dataStore->vpn_rule_white = ui->whitelist_mode->isChecked();
    bool isInternalChanged = NekoGui::dataStore->vpn_internal_tun != ui->single_core->isChecked();
    NekoGui::dataStore->vpn_internal_tun = ui->single_core->isChecked();
    //
    D_SAVE_STRING_PLAIN(vpn_rule_cidr)
    D_SAVE_STRING_PLAIN(vpn_rule_process)
    //
    QStringList msg{"UpdateDataStore"};
    if (isInternalChanged) {
        msg << "NeedRestart";
    } else {
        msg << "VPNChanged";
    }
    MW_dialog_message("", msg.join(","));
    QDialog::accept();
}

void DialogVPNSettings::on_troubleshooting_clicked() {
    auto r = QMessageBox::information(this, tr("Troubleshooting"),
                                      tr("If you have trouble starting VPN, you can force reset nekobox_core process here.\n\n"
                                         "If still not working, see documentation for more information.\n"
                                         "https://github.com/elizqmill/nekoray#readme"),
                                      tr("Reset"), tr("Cancel"), "",
                                      1, 1);
    if (r == 0) {
        GetMainWindow()->StopVPNProcess(true);
    }
}
