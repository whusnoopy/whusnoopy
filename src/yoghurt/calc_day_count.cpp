#include <stdio.h>
#include <math.h>

#include "interface.h"
#include "day_utils.h"

// TODO: translate this functions to well structured sytle
// TODO: 还没仔细看这一堆函数, 他们所做的事情, 他们的输入输出分别是什么

//  根据债券的起息日和结算日返回  上一个付息日和下一个付息日 
// TODO: 传入传出参数分别为? 如果返回值是 struct, 推荐直接传引用
struct date get_value_date(bond_content bond,
        date value_date,
        date settlement_date,
        date &previous_value_date,
        date &next_value_date) {
    struct date another_value_date;

    value_date.year=settlement_date.year;

    // TODO: frequence == 1 时表示用什么方法? 如果这个方法这么长, 那么就应该
    // 将其独立出来作为一个函数, 然后调用之
    if (bond.frequence==1) {
        // 这是一个代码块 (Code Block), 应该注明下这块能干啥, 然后用空行和
        // 其余代码块分开
        if(cmp_date(value_date,settlement_date)>0) {
            next_value_date=value_date;
        } else if(cmp_date(value_date,settlement_date)<=0) {
            // 上面这个判断似乎有点多余

            // TODO: 重新写了个 next_year_date 和 prev_year_date 函数来替代这一堆操作
            // TODO: 把这里都替换掉
            next_value_date.year=value_date.year+1;
            next_value_date.month=value_date.month;
            next_value_date.day=value_date.day;
            /*
             * TODO: 按理这里不会超, 只有二月才有可能, 不然那个 value_date 就是有问题的
             * TODO: 这里可以写成一个日期纠错函数
            if(next_value_date.day>month_days[next_value_date.month]){
                if(is_even_year(next_value_date.year)==1){
                    month_days[2]=29;
                    // TODO: 这里不应该直接这么去修改这个值, 因为 month_days
                    // 是常量数组
                    // TODO: 而且如果想要表达二月, 用 FEB 这样的常量会更好
                }                          
                next_value_date.day=month_days[next_value_date.month];
                month_days[2]=28;                           
            }
            */

            correct_date(next_value_date);
        }               

        if(cmp_date(value_date,settlement_date)>0){
            previous_value_date.year=value_date.year-1;
            previous_value_date.month=value_date.month;
            previous_value_date.day=value_date.day;

            correct_date(previous_value_date);

        } else if(cmp_date(value_date,settlement_date)<0){
            previous_value_date=value_date;
        } else {
            previous_value_date=settlement_date;
        }
    }
    if (bond.frequence==2){
        if(value_date.month<=6){
            another_value_date.year=value_date.year;
            another_value_date.month=value_date.month+6;
            another_value_date.day=value_date.day;

            correct_date(another_value_date);

            if(cmp_date(value_date,settlement_date)>0){
                next_value_date=value_date;
                previous_value_date.year=another_value_date.year-1;
                previous_value_date.month=another_value_date.month;
                previous_value_date.day=another_value_date.day;

                correct_date(previous_value_date);

            } else if(cmp_date(another_value_date,settlement_date)>0&&cmp_date(value_date,settlement_date)<=0){
                next_value_date=another_value_date;
                previous_value_date=value_date;
            } else if(cmp_date(another_value_date,settlement_date)<=0){
                next_value_date.year=value_date.year+1;
                next_value_date.month=value_date.month;
                next_value_date.day=value_date.day;

                correct_date(next_value_date);

                previous_value_date=another_value_date; 
            }
        } else if(value_date.month>6){
            another_value_date.year=value_date.year;
            another_value_date.month=value_date.month-6;
            another_value_date.day=value_date.day;

            correct_date(another_value_date);

            if(cmp_date(another_value_date,settlement_date)>0){
                next_value_date=another_value_date;
                previous_value_date.year=value_date.year-1;
                previous_value_date.month=value_date.month;
                previous_value_date.day=value_date.day;

                correct_date(previous_value_date);

            } else if(cmp_date(value_date,settlement_date)>0&&cmp_date(another_value_date,settlement_date)<=0){
                next_value_date=value_date;
                previous_value_date=another_value_date; 
            } else if(cmp_date(value_date,settlement_date)<=0){
                next_value_date.year=another_value_date.year+1;
                next_value_date.month=another_value_date.month;
                next_value_date.day=another_value_date.day;

                correct_date(next_value_date);

                previous_value_date=value_date;   
            }                          

        } 
    }
}


//根据不同天数计算方法返回 结算日和下一个付息日之间天数  和  两个付息日之间天数 
int calc_day_count(bond_content bond,date value_date,date settlement_date,
        int &day_inter,
        int &coupon_inter){

    struct date previous_value_date;
    struct date next_value_date;

    get_value_date(bond,value_date,settlement_date,previous_value_date,next_value_date);

    // TODO: 如果出错, 则应该退出且返回一个错误代码
    if (cmp_date(value_date, settlement_date) > 0) {
        printf("erro");
        return -1;
    }

    switch(bond.way_of_day_count) 
    {
        // TODO: 把计算方法全部用常量来区分, 代码可读性变高, 至少知道每个是干啥的
        // TODO: 其实加上注释可能会更好, 当然, 可能在你们行业这些单词的意思是应该每个人都知道的
        // TODO: case 后面是不用接 {} 的, 后面天然就是语句块
        case GERMAN:
        {
            if (settlement_date.day==31||settlement_date.day==29||settlement_date.day==28)
                settlement_date.day=30;
            if (next_value_date.day==31||next_value_date.day==29||next_value_date.day==28)
                next_value_date.day=30;
            day_inter=360*(next_value_date.year-settlement_date.year)+30*(next_value_date.month-settlement_date.month)+next_value_date.day-settlement_date.day;
            coupon_inter=360/bond.frequence;
            break;
        }
        case SPECIAL_GERMAN:
        {
            if (settlement_date.day==31)
                settlement_date.day=30;
            if (next_value_date.day==31)
                next_value_date.day=30;
            day_inter=360*(next_value_date.year-settlement_date.year)+30*(next_value_date.month-settlement_date.month)+next_value_date.day-settlement_date.day;
            coupon_inter=360/bond.frequence;
            break;
        } 
        case U30_360:
        {
            if (settlement_date.day==31)
                settlement_date.day=30;
            if ((next_value_date.day==31&&settlement_date.day==31)||(next_value_date.day==31&&settlement_date.day==30))
                next_value_date.day=30;
            day_inter=360*(next_value_date.year-settlement_date.year)+30*(next_value_date.month-settlement_date.month)+next_value_date.day-settlement_date.day;
            coupon_inter=360/bond.frequence;
            break;            
        }

        case ACT_ACT:
        {
            day_inter=calc_act(settlement_date,next_value_date); 
            coupon_inter=calc_act(previous_value_date,next_value_date);
            break;
        }
        case ACT_360:
        {
            day_inter=calc_act(settlement_date,next_value_date);
            coupon_inter=360/bond.frequence;
            break; 
        }         
        case ACT_365:
        {
            day_inter=calc_act(settlement_date,next_value_date);
            coupon_inter=365/bond.frequence;
            break;  
        }

    }           
}

// 计算剩余的付息次数  
int  get_coupon_times(bond_content bond,date value_date,date settlement_date,date maturity_date){
    struct date next_value_date,previous_value_date;
    int coupon_times;
    get_value_date(bond,value_date,settlement_date,previous_value_date,next_value_date);

    // TODO: 这里又出现两次 frequence 不明不白的表达
    if(bond.frequence==1){
        coupon_times=maturity_date.year-next_value_date.year+1;
    }
    if(bond.frequence==2){
        if(next_value_date.month!=maturity_date.month){
            if(next_value_date.month<=6){
                coupon_times=2*(maturity_date.year-next_value_date.year)+2;
            }
            else{
                coupon_times=2*(maturity_date.year-next_value_date.year);
            }

        } 
        else{
            coupon_times=2*(maturity_date.year-next_value_date.year)+1;
        }

    }
    return(coupon_times);

}

//求解到期收益率的方程                 
double get_func(bond_content bond,double yield,double price,int coupon_times,
        int day_inter,
        int coupon_inter){               

    double f=bond.face/pow((1+yield/bond.frequence),(day_inter*1.0/coupon_inter+coupon_times-1))-price;
    for (int i =0;i<coupon_times;i++)

        f+=bond.coupon/bond.frequence/pow((1+yield/bond.frequence),(day_inter*1.0/coupon_inter+i));

    return f;
}
//求解到期收益率的方程的导数 
double get_de_func(bond_content bond,double yield,double price,int coupon_times,
        int day_inter,
        int coupon_inter){

    double de_f=-(day_inter*1.0/coupon_inter+coupon_times-1)*bond.face/pow(1+yield/bond.frequence,day_inter*1.0/coupon_inter+coupon_times)/bond.frequence;
    for (int i=0;i<coupon_times;i++)

        de_f+=-(day_inter*1.0/coupon_inter+i)*bond.coupon/bond.frequence/pow((1+yield)/bond.frequence,day_inter*1.0/coupon_inter+i+1)/bond.frequence;

    return de_f;
}

//求解固定利率债券到期收益率            
double calc_yield(bond_content bond,double price,date value_date,date settlement_date,date maturity_date){


    int day_inter,coupon_inter;
    double x0=0.07,x=0;

    calc_day_count(bond,value_date,settlement_date,day_inter,coupon_inter);
    double left_years=calc_act(settlement_date,maturity_date)/365;    
    int coupon_times=get_coupon_times(bond,value_date,settlement_date,maturity_date);

    if((coupon_times!=1)||(left_years>1)) {    
        while(fabs(x-x0)>=EPS)
        {
            x0=x;
            x=x0-get_func(bond,x0,price,coupon_times,day_inter,coupon_inter)/get_de_func(bond,x0,price,coupon_times,day_inter,coupon_inter);

        }
        return(x);
    }
    //到期期限在1年以内且只有一次付息的债券     单利计算到期收益率
    else if((coupon_times==1)&&(left_years<=1)){

        return((bond.face+bond.coupon/bond.frequence-price)*coupon_inter/price/day_inter);
    }



}


//求解固定利率债券价格 
double calc_price(bond_content bond,double yield,date value_date,date settlement_date,date maturity_date){

    struct date previous_value_date,next_value_date;
    int day_inter,coupon_inter;
    int coupon_times;

    coupon_times=get_coupon_times(bond,value_date,settlement_date,maturity_date);

    double price=bond.face/pow((1+yield/bond.frequence),(day_inter*1.0/coupon_inter+coupon_times-1));
    double sum=(bond.face+bond.coupon/bond.frequence)/pow((1+yield/bond.frequence),(day_inter*1.0/coupon_inter+coupon_times-1));
    for (int j=0;j<=coupon_times-1;j++)

        price+=bond.coupon/bond.frequence/pow((1+yield/bond.frequence),(day_inter*1.0/coupon_inter+j));

    return(price);
}




//求解债券久期    
double  calc_duration(bond_content bond,double price,date value_date,date settlement_date,date maturity_date,double &macaulay_d,double &modified_d,double &DV01,double &effective_d,double &p1,double &p2){

    double yield=calc_yield(bond,price,value_date,settlement_date,maturity_date);
    int coupon_times=get_coupon_times(bond,value_date,settlement_date,maturity_date);
    int day_inter,coupon_inter;
    calc_day_count(bond,value_date,settlement_date,day_inter,coupon_inter);

    macaulay_d=bond.face*coupon_times*1.0/pow(1+yield/bond.frequence,coupon_times);
    p1=(bond.face+bond.coupon/bond.frequence)*1.0/pow(1+yield/bond.frequence,coupon_times);
    p2=p1*coupon_times;
    for (int i =1;i<=coupon_times;i++){

        macaulay_d+=bond.coupon*i*1.0/bond.frequence/pow(1+yield/bond.frequence,i);  
    }       //麦考雷久期 



    macaulay_d=macaulay_d/price;
    modified_d=macaulay_d/(1+yield/bond.frequence)/bond.frequence;   //    修正久期 
    macaulay_d=macaulay_d/bond.frequence;                     //     麦考雷久期（年） 

    DV01 =modified_d*price/10000;    // DV01

    //p1=calc_price(bond,yield-0.001,value_date,settlement_date,maturity_date);
    //p2=calc_price(bond,yield+0.001,value_date,settlement_date,maturity_date);

    effective_d=(calc_price(bond,yield-0.001,value_date,settlement_date,maturity_date)-calc_price(bond,yield+0.001,value_date,settlement_date,maturity_date))/(2*0.002*price);



}

//求解债券凸性 
double calc_convexity(bond_content bond,double price,date value_date,date settlement_date,date maturity_date,double &convexity,double &effective_convexity,double &p3,double &p4){

    double yield=calc_yield(bond,price,value_date,settlement_date,maturity_date);
    int coupon_times=get_coupon_times(bond,value_date,settlement_date,maturity_date);
    convexity=bond.face*coupon_times*(coupon_times+1)/pow(1+yield/bond.frequence,coupon_times+2);
    //p3=(bond.face+bond.coupon/bond.frequence)*coupon_times*(coupon_times+1)/pow(1+yield/bond.frequence,coupon_times);
    p3=(bond.coupon/bond.frequence)*(coupon_times-1)*coupon_times/pow(1+yield/bond.frequence,coupon_times-1);
    p4=(bond.face+bond.coupon/bond.frequence)/pow(1+yield/bond.frequence,coupon_times);


    for (int i=1;i<=coupon_times;i++)
    {
        convexity+=bond.coupon/bond.frequence*i*(i+1)/pow(1+yield/bond.frequence,i+2);         //凸度
    } 

    convexity=convexity/price/pow(bond.frequence,2);
    effective_convexity=(calc_price(bond,yield-0.001,value_date,settlement_date,maturity_date)+calc_price(bond,yield+0.001,value_date,settlement_date,maturity_date)-2*price)/(2*price*pow(0.002,2));
}


//求解利随本清债券的到期收益率 
double calc_yield_all(bond_content bond,double price,date value_date,date settlement_date,date maturity_date){         


    int day_inter,coupon_inter;
    double left_years=calc_act(settlement_date,maturity_date)/365;   

    calc_day_count(bond,value_date,settlement_date,day_inter,coupon_inter);



    //到期期限在1年以内且只有一次付息的债券     单利计算到期收益率
    if(left_years<=1){            
        return((bond.face+bond.coupon*bond.maturity-price)*coupon_inter*1.0/price/day_inter);
    }
    else if(left_years>1){
        return(pow((bond.face+bond.coupon*bond.maturity)/price,(1/(day_inter*1.0/coupon_inter+int(left_years))))-1);
    }
}

//求解贴现式债券（零息债券）的到期收益率 
double calc_yield_zero(bond_content bond,double price,date value_date,date settlement_date,date maturity_date){


    int day_inter,coupon_inter;
    double left_years=calc_act(settlement_date,maturity_date)/365;

    calc_day_count(bond,value_date,settlement_date,day_inter,coupon_inter);



    //到期期限在1年以内且只有一次付息的债券     单利计算到期收益率
    if(left_years<=1){            
        return((bond.face-price)*coupon_inter*1.0/price/day_inter);
    }
    else if(left_years>1){
        return(pow(bond.face/price,(1/(day_inter*1.0/coupon_inter+int(left_years))))-1);
    }
}

int main(int argc, char* argv[]) {
    struct date value_date,settlement_date,maturity_date,previous_value_date,next_value_date;
    int day_inter,coupon_inter;
    double price,yield,macaulay_d,modified_d,DV01,effective_d,convexity,effective_convexity;
    int coupon_times;
    double p1,p2,p3,p4;

    value_date.year = 2001;
    value_date.month = 11;
    value_date.day = 8;

    settlement_date.year = 2001;
    settlement_date.month = 11;
    settlement_date.day = 8;

    maturity_date.year=2006;
    maturity_date.month=11;
    maturity_date.day=8;

    struct bond_content bond;
    bond.way_of_day_count=3;
    bond.frequence=2;
    bond.coupon=8;
    bond.face=100;
    //bond.maturity=7;
    price=96.04364;
    //yield=0.11;
    double sum;


    calc_day_count(bond,value_date,settlement_date,day_inter,coupon_inter);
    double w=day_inter*1.0/coupon_inter;
    get_value_date(bond,value_date,settlement_date,previous_value_date,next_value_date);

    //price=calc_price(bond,yield,value_date,settlement_date,maturity_date);
    yield=calc_yield(bond,price,value_date,settlement_date,maturity_date);
    coupon_times=get_coupon_times(bond,value_date,settlement_date,maturity_date);
    calc_duration(bond,price,value_date,settlement_date,maturity_date,macaulay_d,modified_d,DV01,effective_d,p1,p2);
    calc_convexity(bond,price,value_date,settlement_date,maturity_date,convexity,effective_convexity,p3,p4);

    printf("previous value date is %d,%d,%d\n",previous_value_date.year,previous_value_date.month,previous_value_date.day);
    printf("next value date is %d,%d,%d\n",next_value_date.year,next_value_date.month,next_value_date.day);
    printf("day interval is %d,coupon interval is %d,left coupon times is %d,%f\n",day_inter,coupon_inter,coupon_times,w);

    printf("macaulay duration is %1.9f,modified duration is %1.9f,DV01 is %f,effective duration is %f,%f,%f\n",macaulay_d,modified_d,DV01,effective_d,p1,p2);
    printf("convexity is %f,effective convexity is %f,%f,%f\n",convexity,effective_convexity,p3,p4);
    printf("yield is %f",yield);
    printf("price is %f",price);

    return 0;
}

