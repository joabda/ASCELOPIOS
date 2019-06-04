/********************************************************************************
** Form generated from reading UI file 'eventdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CATEGORYEDITDIALOG_H
#define UI_CATEGORYEDITDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>

QT_BEGIN_NAMESPACE

class Ui_CategoryEditDialog
{
public:

    void setupUi(QDialog *CategoryEditDialog)
    {
        if (CategoryEditDialog->objectName().isEmpty())
            CategoryEditDialog->setObjectName(QStringLiteral("CategoryEditDialog"));
        CategoryEditDialog->resize(400, 300);

        retranslateUi(CategoryEditDialog);

        QMetaObject::connectSlotsByName(CategoryEditDialog);
    } // setupUi

    void retranslateUi(QDialog *CategoryEditDialog)
    {
        CategoryEditDialog->setWindowTitle(QApplication::translate("CategoryEditDialog", "Dialog", 0));
    } // retranslateUi

};

namespace Ui {
    class CategoryEditDialog: public Ui_CategoryEditDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CATEGORYEDITDIALOG_H
