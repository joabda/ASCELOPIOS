/********************************************************************************
** Form generated from reading UI file 'kalendar.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_KALENDAR_H
#define UI_KALENDAR_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Kalendar
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *Kalendar)
    {
        if (Kalendar->objectName().isEmpty())
            Kalendar->setObjectName(QStringLiteral("Kalendar"));
        Kalendar->resize(400, 300);
        menuBar = new QMenuBar(Kalendar);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        Kalendar->setMenuBar(menuBar);
        mainToolBar = new QToolBar(Kalendar);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        Kalendar->addToolBar(mainToolBar);
        centralWidget = new QWidget(Kalendar);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        Kalendar->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(Kalendar);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        Kalendar->setStatusBar(statusBar);

        retranslateUi(Kalendar);

        QMetaObject::connectSlotsByName(Kalendar);
    } // setupUi

    void retranslateUi(QMainWindow *Kalendar)
    {
        Kalendar->setWindowTitle(QApplication::translate("Kalendar", "Kalendar", 0));
    } // retranslateUi

};

namespace Ui {
    class Kalendar: public Ui_Kalendar {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_KALENDAR_H
