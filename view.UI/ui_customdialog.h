/********************************************************************************
** Form generated from reading UI file 'eventdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CUSTOMDIALOG_H
#define UI_CUSTOMDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>

QT_BEGIN_NAMESPACE

class Ui_CustomDialog
{
public:

    void setupUi(QDialog *CustomDialog)
    {
        if (CustomDialog->objectName().isEmpty())
            CustomDialog->setObjectName(QStringLiteral("CustomDialog"));
        CustomDialog->resize(400, 300);

        CustomDialog->setWindowTitle(QApplication::translate("CustomDialog", "Dialog", 0));

        QMetaObject::connectSlotsByName(CustomDialog);
    } // setupUi

};

namespace Ui {
    class CustomDialog: public Ui_CustomDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif
