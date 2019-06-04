/********************************************************************************
** Form generated from reading UI file 'eventdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CATEGORYDIALOG_H
#define UI_CATEGORYDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>

QT_BEGIN_NAMESPACE

class Ui_CategoryDialog
{
public:

    void setupUi(QDialog *CategoryDialog)
    {
        if (CategoryDialog->objectName().isEmpty())
            CategoryDialog->setObjectName(QStringLiteral("CategoryDialog"));
        CategoryDialog->resize(400, 300);

        retranslateUi(CategoryDialog);

        QMetaObject::connectSlotsByName(CategoryDialog);
    } // setupUi

    void retranslateUi(QDialog *CategoryDialog)
    {
        CategoryDialog->setWindowTitle(QApplication::translate("CategoryDialog", "Dialog", 0));
    } // retranslateUi

};

namespace Ui {
    class CategoryDialog: public Ui_CategoryDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CATEGORYDIALOG_H
