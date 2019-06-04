#ifndef VIEW_H
#define VIEW_H

#include "model/date.h"
#include "model/category.h"

class View
{
public:
    virtual void refresh_events() = 0;
    virtual void display_events(Date date, Category* category) = 0;
    virtual ~View() {}
};

#endif // VIEW_H
