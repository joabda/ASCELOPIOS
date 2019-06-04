#ifndef CUSTOMDIALOG_H
#define CUSTOMDIALOG_H

#include "../view.UI/ui_customdialog.h"
#include <QDialog>
#include <QKeyEvent>
#include <QLabel>

namespace Ui {
class CustomDialog;
}

class CustomDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CustomDialog(QLayout* layout, QWidget* parent = nullptr);
    ~CustomDialog();

protected:
    virtual void keyPressEvent(QKeyEvent* e);

private:
    Ui::CustomDialog* ui;
};

#endif
