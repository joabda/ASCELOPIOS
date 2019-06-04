#ifndef LINUXNOTIFYMANAGER_H
#define LINUXNOTIFYMANAGER_H

#include <QProcess>
#include "pmanager.h"
#include "notifymanager.h"
#include "dateutil.h"

#define ICON_NAME "appointment-new" /* TODO: choose a better icon */

class LinuxNotifyManager : public NotifyManager
{
public:
    LinuxNotifyManager();
    bool notifyEvents(const int dayoffset);
    ~LinuxNotifyManager() {}
private:
    bool notifyEvent(Event* e);
};

#endif // LINUXNOTIFYMANAGER_H
