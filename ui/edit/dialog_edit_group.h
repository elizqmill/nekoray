#pragma once

#include <QDialog>
#include "db/Group.hpp"

QT_BEGIN_NAMESPACE
namespace Ui {
    class DialogEditGroup;
}
QT_END_NAMESPACE

class DialogEditGroup : public QDialog {
    Q_OBJECT

public:
    explicit DialogEditGroup(const std::shared_ptr<NekoGui::Group> &ent, QWidget *parent = nullptr);

    ~DialogEditGroup() override;

private:
    Ui::DialogEditGroup *ui;

    std::shared_ptr<NekoGui::Group> ent;

    struct {
        int front_proxy;
    } CACHE;

    QString request_headers_cache;

    void refresh_front_proxy();

    void refresh_request_headers();

private slots:

    void accept() override;

    void on_front_proxy_clicked();

    void on_request_headers_clicked();
};
