#ifndef NOTIFYMANAGER_H
#define NOTIFYMANAGER_H


class NotifyManager
{
public:
    virtual bool notifyEvents(int day) = 0;
    virtual ~NotifyManager() {}
};

#endif // NOTIFYMANAGER_H
