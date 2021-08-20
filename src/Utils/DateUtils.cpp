#include "Utils/DateUtils.hpp"
#include <ctime>

namespace DateUtils
{
    tm* get_time()
    {
        time_t ttime = time(0);

        tm* gmt_time = localtime(&ttime);

        return gmt_time;
    }

    bool get_isMonth(int month)
    {
        return (get_time()->tm_mon + 1) == month;
    }

    bool get_isDay(int day)
    {
        return get_time()->tm_mday == day;
    }

    bool get_isMonthAndDay(int month, int day)
    {
        return get_isMonth(month) && get_isDay(day);
    }
}