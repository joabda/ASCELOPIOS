#ifndef DATEUTIL_H
#define DATEUTIL_H

#include <time.h>
#include <string>
#include <list>

#include "../model/date.h"

#define SECONDS_IN_1DAY 86400

using namespace std;

class DateUtil
{
public:
    DateUtil();
    static Date get_current_date();
    static string get_literal_month(const int monthNumber);
    static string numeric2literal_day_of_week(const int dayNumber);
    static int literal2numeric_day_of_week(const string &d);
    static int get_days_in_month(const int monthNumber, const int year);
    static Date date_from_timestamp(const time_t& timestamp);
    static Date get_first_day_of_month(Date& date);
    static Date get_last_day_of_month(Date& date);
    static Date increase_month(Date date);
    static Date decrease_month(Date date);
    static Date increase_day(Date date);
    static Date decrease_day(Date date);
    static Date increase_year(Date date);
    static Date decrease_year(Date date);
    static bool is_leap(const int year);
};

#endif
