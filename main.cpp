#include <QApplication>
#include <QDebug>
#include <QCommandLineParser>
#include <QPushButton>
#include <QHBoxLayout>
#include "pmanager.h"
#include "login.h"
#include "view/monthview.h"
#include "util/linuxnotifymanager.h"
#include "util/eventutil.h"
#include "test/test.h"

#define RUN_TESTS 0

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    int ret = 0;
    #if RUN_TESTS
    // Test t;
    // t.test_persistence();
    // t.test_util();
    #else
    bool cli = false;
    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addOptions({
            {{"n", "notify"},
                QCoreApplication::translate("main", "Notify the events in the future <dayoffset> starting from today"),
                QCoreApplication::translate("main", "notifynextdays")},
            {{"a", "add"},
                QCoreApplication::translate("main", "Add an event"),
                QCoreApplication::translate("main", "event")},
            {{"d", "delete"},
                QCoreApplication::translate("main", "Delete an event"),
                QCoreApplication::translate("main", "event")},
        });
    parser.process(a);
    if ((cli = parser.isSet("add")))
    {
        QString event = parser.value("add");
        PManager spm;
        spm.add_event(EventUtil::parseString(event.toStdString()));
    } else if((cli = parser.isSet("delete")))
    {
        /* TODO: implement deletion by id */
        QString eventName = parser.value("delete");
        PManager spm;
        for (Event* e : spm.get_all_events())
            if (e->getName() == eventName.toStdString())
                spm.remove_event(e);
    }
    else
        if ((cli = parser.isSet("notify")))
        {
            /* Show notifications about the events in the next days */
            QString notify =  parser.value("notify");
            LinuxNotifyManager nm;
            if (!nm.notifyEvents(notify.toInt()))
            {
                printf("Error in notifyEvents");
                ret = 1;
            }
    }

    Login loginDialog;
    loginDialog.exec();
    MonthView window;
    window.setWindowTitle("ASCELOPIOS");
    if (!cli && loginDialog.logedIn())
    {
        window.show();
        ret = a.exec();
    }
    #endif
    return ret;
}
