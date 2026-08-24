#pragma once

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui {
    class DialogRealityGen;
}
QT_END_NAMESPACE

class DialogRealityGen : public QDialog {
    Q_OBJECT

public:
    explicit DialogRealityGen(QWidget *parent = nullptr);

    ~DialogRealityGen() override;

private slots:

    void on_uuidgen_clicked();

    void on_sidgen_clicked();

    void on_copy_clicked();

private:
    Ui::DialogRealityGen *ui;
};
