#pragma once

#include <QWidget>
#include "profile_editor.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class EditTailscale;
}
QT_END_NAMESPACE

class EditTailscale : public QWidget, public ProfileEditor {
    Q_OBJECT

public:
    explicit EditTailscale(QWidget *parent = nullptr);

    ~EditTailscale() override;

    void onStart(std::shared_ptr<NekoGui::ProxyEntity> ent) override;

    bool onEnd() override;

private:
    Ui::EditTailscale *ui;
    std::shared_ptr<NekoGui::ProxyEntity> ent;
};
