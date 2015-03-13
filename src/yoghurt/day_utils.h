#ifndef __DAY_UTILS_H__
#define __DAY_UTILS_H__

struct date
{
    int year;
    int month;
    int day;
};

const int DAYS_IN_A_YEAR = 365;

enum MONTH {
    FIRST_MONTH = 1,
    JAN = 1,
    FEB,
    MAR,
    APR,
    MAY,
    JUN,
    JUL,
    AUG,
    SEP,
    OCT,
    NOV,
    DEC,
    LAST_MONTH = 12,
};

// days in each month
const int month_days[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

// 闰年返回 1 ，不是闰年返回 0 
int is_even_year(int year) {
    if (year % 4 != 0) {
        return 0;
    }
    if (year % 100 == 0 && year % 400 != 0) {
        return 0;
    }
    return 1;
}

// 计算 date1 和 date2 之间的天数 
int calc_act(date date1, date date2) {
    int days = 0;

    // in the same year
    if (date2.year == date1.year) {
        if (date2.month == date1.month) {
            days += date2.day - date1.day;
        } else {
            days += month_days[date1.month] - date1.day;
            if (date1.month == FEB) {
                days += is_even_year(date1.year);
            }
            for (int month = date1.month + 1; month < date2.month; ++month) {
                days += month_days[month];
                if (month == FEB) {
                    days += is_even_year(date1.year);
                }
            }
            days += date2.day;
        }
    } else {
        // calc days from date1 to the last day in date1's year
        days += month_days[date1.month] - date1.day;
        if (date1.month == FEB) {
            days += is_even_year(date1.year);
        }
        for (int month = date1.month + 1; month <= LAST_MONTH; ++month) {
            days += month_days[month];
            if (month == FEB) {
                days += is_even_year(date1.year);
            }
        }

        // calc days between date1's last day to date2's first day
        for (int year = date1.year + 1; year < date2.year; ++year) {
            days += DAYS_IN_A_YEAR;
            days += is_even_year(year);
        }

        // calc days from the first day of date2 to date2
        for (int month = FIRST_MONTH; month < date2.month; ++month) {
            days += month_days[month];
            if (month == FEB) {
                days += is_even_year(date2.year);
            }
        }
        days += date2.day;
    }

    return days;
}

// 比较两个日期的大小 
// return -1 if date1 is early than date2
// return 1 if date1 is later than date2
// return 0 if date1 is the same day of date2
int cmp_date(date date1, date date2) {
    if (date1.year > date2.year) {
        return 1;
    } else if (date1.year < date2.year) {
        return -1;
    }

    if (date1.month > date2.month) {
        return 1;
    } else if (date1.month < date2.month) {
        return -1;
    }

    if (date1.day > date2.day) {
        return 1;
    } else if (date1.day < date2.day) {
        return -1;
    }

    return 0;
}

// 日期纠错, 看是否有超过最大月份或者当月最大天数的
// return 0: 不用纠错
// return 1: 月份最大超出, 纠错为当年最后一天
// return 2: 天数最大超出, 纠错为当月最后一天
int correct_date(date &org_date) {
    if (org_date.month > LAST_MONTH) {
        org_date.month = LAST_MONTH;
        org_date.day = month_days[LAST_MONTH];
        return 1;
    }

    if (org_date.day > month_days[org_date.month]) {
        if (org_date.month == FEB) {
            int tmp_days = month_days[FEB] + is_even_year(org_date.year);
            if (org_date.day > tmp_days) {
                org_date.day = tmp_days;
            } else {
                // 没错
                return 0;
            }
        } else {
            org_date.day = month_days[org_date.month];
        }
        return 2;
    }

    return 0;
}

// 返回前一年的日期
date prev_year_date(date cur_date) {
    date new_date = cur_date;
    new_date.year--;
    correct_date(new_date);

    return new_date;
}

// 返回后一年的日期
date next_year_date(date cur_date) {
    date new_date = cur_date;
    new_date.year++;
    correct_date(new_date);

    return new_date;
}

#endif

