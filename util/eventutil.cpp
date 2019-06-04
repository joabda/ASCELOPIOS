#include "eventutil.h"

#include <QDebug>

Event* EventUtil::parseString(const string &str_event)
{
    string str = str_event;
    size_t pos = 0;
    string token[6];
    string delimiter("##");
    int i = 0;
    while ((pos = str.find(delimiter)) != std::string::npos)
    {
        token[i] = str.substr(0, pos);
        str.erase(0, pos + delimiter.length());
        i++;
    }
    if ((i < 5) || (token[0] == ""))
        return nullptr;
    token[5] = str;
    Category* category = nullptr;
    PManager spm;
    vector<Category*> categories = spm.get_categories();
    for (Category* c : categories)
        if (token[3] == c->getName())
        {
            category = new Category(*c);
            break;
        }

    return new Event(0,token[0],token[1],token[2],category,stol(token[4]),stol(token[5]));
}
