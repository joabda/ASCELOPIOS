#-------------------------------------------------
#
# Project created by QtCreator 2016-03-09T17:50:52
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Kalendar
TEMPLATE = app


SOURCES += main.cpp\
    login.cpp \
    patient/patient.cpp \
    patient/patientmanager.cpp \
    patient/pregnantpatient.cpp \
    pmanager.cpp \
    view/monthview.cpp \
    util/dateutil.cpp \
    view/eventdialog.cpp \
    view/qwidget_extended.cpp \
    view/qlabel_event.cpp \
    view/categorydialog.cpp \
    view/qpushbutton_extended.cpp \
    view/customdialog.cpp \
    view/categoryeditdialog.cpp \
    view/categoryselectdialog.cpp \
    util/linuxnotifymanager.cpp \
    util/eventutil.cpp \
    util/pluginmanager.cpp \
    view/settings.cpp

HEADERS  += model/event.h \
    login.h \
    patient/patient.h \
    patient/patientmanager.h \
    patient/pregnantpatient.h \
    model/category.h \
    model/category.h \
    model/event.h \
    pmanager.h \
    view.UI/ui_categorydialog.h \
    view.UI/ui_categoryeditdialog.h \
    view.UI/ui_categoryselectdialog.h \
    view.UI/ui_customdialog.h \
    view.UI/ui_eventdialog.h \
    view.UI/ui_kalendar.h \
    view.UI/ui_monthview.h \
    view.UI/ui_settings.h \
    view/monthview.h \
    view/qframe_extended.h \
    model/date.h \
    util/dateutil.h \
    view/eventdialog.h \
    view/qwidget_extended.h \
    view/qlabel_event.h \
    view/view.h \
    view/categorydialog.h \
    view/qpushbutton_extended.h \
    view/customdialog.h \
    view/categoryeditdialog.h \
    view/categoryselectdialog.h \
    util/notifymanager.h \
    util/linuxnotifymanager.h \
    util/eventutil.h \
    util/pluginmanager.h \
    view/settings.h

QMAKE_LIBS += -lsqlite3
QMAKE_LIBS += -lstdc++fs
QMAKE_CXXFLAGS += -std=c++11

#For DEBUG purpose
#QMAKE_CXXFLAGS += -g

FORMS += \
    login.ui \
    patient/patientmanager.ui

RESOURCES += \
    resource.qrc
