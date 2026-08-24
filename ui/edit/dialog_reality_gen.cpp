#include "dialog_reality_gen.h"
#include "ui_dialog_reality_gen.h"

#include <QClipboard>
#include <QRandomGenerator>
#include <QUrl>
#include <QUrlQuery>
#include <QUuid>

DialogRealityGen::DialogRealityGen(QWidget *parent) : QDialog(parent), ui(new Ui::DialogRealityGen) {
    ui->setupUi(this);
    on_uuidgen_clicked();
}

DialogRealityGen::~DialogRealityGen() {
    delete ui;
}

void DialogRealityGen::on_uuidgen_clicked() {
    ui->uuid->setText(QUuid::createUuid().toString().remove("{").remove("}"));
}

void DialogRealityGen::on_sidgen_clicked() {
    // short id: up to 8 bytes hex
    QString sid;
    for (int i = 0; i < 8; i++) {
        sid += QString::number(QRandomGenerator::global()->bounded(16), 16);
    }
    ui->short_id->setText(sid);
}

void DialogRealityGen::on_copy_clicked() {
    auto uuid = ui->uuid->text().trimmed();
    auto address = ui->address->text().trimmed();
    auto port = ui->port->text().trimmed();
    if (uuid.isEmpty() || address.isEmpty()) return;
    if (port.isEmpty()) port = "443";

    QUrl url;
    url.setScheme("vless");
    url.setUserName(uuid);
    url.setHost(address, QUrl::StrictMode);
    url.setPort(port.toInt());
    QUrlQuery q;
    q.addQueryItem("encryption", "none");
    auto flow = ui->flow->currentText();
    if (!flow.isEmpty()) q.addQueryItem("flow", flow);
    q.addQueryItem("security", "reality");
    auto sni = ui->sni->text().trimmed();
    if (!sni.isEmpty()) q.addQueryItem("sni", sni);
    q.addQueryItem("fp", ui->fingerprint->currentText());
    auto pbk = ui->public_key->text().trimmed();
    if (!pbk.isEmpty()) q.addQueryItem("pbk", pbk);
    auto sid = ui->short_id->text().trimmed();
    if (!sid.isEmpty()) q.addQueryItem("sid", sid);
    q.addQueryItem("type", "tcp");
    url.setQuery(q);
    auto name = ui->name->text().trimmed();
    if (!name.isEmpty()) url.setFragment(name);

    auto link = url.toString(QUrl::FullyEncoded);
    ui->result->setPlainText(link);
    QApplication::clipboard()->setText(link);
}
