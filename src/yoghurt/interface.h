#ifndef __INTERFACE_H__
#define __INTERFACE_H__

const double EPS = 1e-9;

// TODO: 其实还是不太明白这里面都是啥意思
enum WAY_OF_DAY_COUNT {
    GERMAN = 1,
    SPECIAL_GERMAN = 2,
    // TODO: 变量/常量名不能用数字开头
    // 30U_360 = 3,
    U30_360 = 3,
    ACT_ACT = 4,
    ACT_360 = 5,
    ACT_365 = 6,
};

struct bond_content  
{
    double face;
    double coupon;
    double maturity;

    // 天数计算方法, 见 WAY_OF_DAY_COUNT
    int way_of_day_count;

    // TODO: 这是啥? 看 get_value_date 里有两个值, 分别指代什么?
    int frequence;

    //   1: 一年以内贴现债券
    //   2: 一次性换本付息债券或零息债券
    //   3: 固定利率付息债券,
    int type;
};

#endif

