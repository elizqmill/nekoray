#pragma once

#include <QWidget>
#include "profile_editor.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class EditAnyTLS;
}
QT_END_NAMESPACE

class EditAnyTLS : public QWidget, public ProfileEditor {
    Q_OBJECT

public:
    explicit EditAnyTLS(QWidget *parent = nullptr);

    ~EditAnyTLS() override;

    void onStart(std::shared_ptr<NekoGui::ProxyEntity> ent) override;

    bool onEnd() override;

private:
    Ui::EditAnyTLS *ui;
    std::shared_ptr<NekoGui::ProxyEntity> ent;
};
