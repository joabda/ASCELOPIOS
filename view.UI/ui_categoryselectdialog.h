/********************************************************************************
** Form generated from reading UI file 'eventdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CATEGORYSELECTDIALOG_H
#define UI_CATEGORYSELECTDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>

QT_BEGIN_NAMESPACE

class Ui_CategorySelectDialog
{
public:

    void setupUi(QDialog *CategorySelectDialog)
    {
        if (CategorySelectDialog->objectName().isEmpty())
            CategorySelectDialog->setObjectName(QStringLiteral("CategorySelectDialog"));
        CategorySelectDialog->resize(400, 300);

        retranslateUi(CategorySelectDialog);

        QMetaObject::connectSlotsByName(CategorySelectDialog);
    } // setupUi

    void retranslateUi(QDialog *CategorySelectDialog)
    {
        CategorySelectDialog->setWindowTitle(QApplication::translate("CategorySelectDialog", "Dialog", 0));
    } // retranslateUi

};

namespace Ui {
    class CategorySelectDialog: public Ui_CategorySelectDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CATEGORYSELECTDIALOG_H
