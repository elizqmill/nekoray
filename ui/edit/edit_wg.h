#pragma once

#include <QWidget>
#include "profile_editor.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class EditWG;
}
QT_END_NAMESPACE

class EditWG : public QWidget, public ProfileEditor {
    Q_OBJECT

public:
    explicit EditWG(QWidget *parent = nullptr);

    ~EditWG() override;

    void onStart(std::shared_ptr<NekoGui::ProxyEntity> ent) override;

    bool onEnd() override;

private:
    Ui::EditWG *ui;
    std::shared_ptr<NekoGui::ProxyEntity> ent;
};
