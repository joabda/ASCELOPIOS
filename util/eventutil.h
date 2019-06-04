#ifndef EVENTUTIL_H
#define EVENTUTIL_H

#include <string>
#include "../model/event.h"
#include "pmanager.h"

using namespace std;

class EventUtil
{
public:
    static Event* parseString(const string& str_event);
};

#endif // EVENTUTIL_H
