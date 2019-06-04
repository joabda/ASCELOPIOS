#include "customdialog.h"

#include <QDebug>

CustomDialog::CustomDialog(QLayout* layout, QWidget* parent) :
    QDialog(parent),
    ui(new Ui::CustomDialog)
{
    setWindowFlags(Qt::WindowCloseButtonHint | Qt::WindowMinimizeButtonHint);
    setLayout(layout);
}

CustomDialog::~CustomDialog()
{
    delete ui;
}

void CustomDialog::keyPressEvent(QKeyEvent* e)
{
    if(e->key() != Qt::Key_Escape)
        QDialog::keyPressEvent(e);
    else
    {
        close();
        delete this;
    }
}
