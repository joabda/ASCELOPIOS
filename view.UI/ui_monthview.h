/********************************************************************************
** Form generated from reading UI file 'monthview.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MONTHVIEW_H
#define UI_MONTHVIEW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MonthView
{
public:
    QMenuBar *menubar;
    QWidget *centralwidget;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MonthView)
    {
        if (MonthView->objectName().isEmpty())
            MonthView->setObjectName(QStringLiteral("MonthView"));
        MonthView->resize(800, 600);
        menubar = new QMenuBar(MonthView);
        menubar->setObjectName(QStringLiteral("menubar"));
        MonthView->setMenuBar(menubar);
        centralwidget = new QWidget(MonthView);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        MonthView->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MonthView);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        MonthView->setStatusBar(statusbar);

        retranslateUi(MonthView);

        QMetaObject::connectSlotsByName(MonthView);
    } // setupUi

    void retranslateUi(QMainWindow *MonthView)
    {
        MonthView->setWindowTitle(QApplication::translate("MonthView", "MainWindow", 0));
    } // retranslateUi

};

namespace Ui {
    class MonthView: public Ui_MonthView {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MONTHVIEW_H
