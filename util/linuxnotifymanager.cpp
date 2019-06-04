#include "linuxnotifymanager.h"

#include <QDebug>

LinuxNotifyManager::LinuxNotifyManager()
{

}

/* Send a notification with all the future events starting from the current date to the specified day */
bool LinuxNotifyManager::notifyEvents(const int dayoffset)
{
    bool ret = true;
    PManager pm;
    Date current = DateUtil::get_current_date();
    Date untildate = current;
    for (int i = 0; i < dayoffset; i++)
        untildate = DateUtil::increase_day(untildate);


    list<Event*> events = pm.get_events_of_month(current.getMonth(), current.getYear());
    if (untildate.getMonth() != current.getMonth()) //if untildate is in the next month, I'll append all the events of the next month
        events.splice(events.end(), pm.get_events_of_month(untildate.getMonth(), untildate.getYear()));
    for (Event* event : events)
    {
        Date start = DateUtil::date_from_timestamp(event->getStart());
        if ((start.compareTo(current) >= 0) && (start.compareTo(untildate) <= 0))
            /* Send a notification */
            ret = ret && this->notifyEvent(event);
    }
    return ret;
}

bool LinuxNotifyManager::notifyEvent(Event* e)
{
    QProcess process;
    Date start = DateUtil::date_from_timestamp(e->getStart());
    Date today = DateUtil::get_current_date();
    Date tomorrow = DateUtil::increase_day(today);
    QString title(start.toString(false).c_str());
    if (start == today)
        title = QString("Today");
    else
        if (start == tomorrow)
            title = QString("Tomorrow");
    process.start("notify-send", QStringList() << "-i" << ICON_NAME << title << e->getName().c_str());
    return process.waitForFinished();
}
