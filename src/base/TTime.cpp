#include"TTime.h"
#include <sys/time.h>

using namespace tmms::base;

int64_t TTime::NowMS(){
    //tv_sec表示秒，tv_usec表示微秒
    //tv表示当前时间到1970年1月1日0时0分0秒的秒数
    struct timeval tv;
    gettimeofday(&tv, NULL);

    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

//表示当前的UTC时间，单位是秒
int64_t TTime::Now(){
    struct timeval tv;
    //获取当前时间，并保存到tv中
    gettimeofday(&tv, NULL);
    return tv.tv_sec;
}

//表示当前时间，返回当前的年月日时分
int64_t TTime::Now(int &year,int &month,int&day,int &hour,int&minute,int&second){
    struct tm tm;
    //获取当前时间
    time_t t = time(NULL);
    //将当前时间转换为本地时间
    localtime_r(&t,&tm);

    year = tm.tm_year + 1900;
    month = tm.tm_mon + 1;
    day = tm.tm_mday;
    hour = tm.tm_hour;
    minute = tm.tm_min;
    second = tm.tm_sec;

    return t;
}

//表示返回当前时间ISO格式的字符串(yyyy-MM-ddTHH:mm:ss)
std::string TTime::ISOTime(){

    struct tm tm;
    // 获取当前时间
    time_t t = time(NULL);
    // 将当前时间转换为本地时间
    localtime_r(&t,&tm);

    // 定义一个字符数组，用于存储格式化后的时间字符串
    char buf[128] = {0};
    // 使用sprintf函数将时间格式化为字符串
    auto n = sprintf(buf, "%04d-%02d-%02dT%02d:%02d:%02d",
        tm.tm_year + 1900, // 年份
        tm.tm_mon + 1, // 月份
        tm.tm_mday, // 日期
        tm.tm_hour, // 小时
        tm.tm_min, // 分钟
        tm.tm_sec // 秒
        );
    // 返回格式化后的时间字符串
    return std::string(buf,buf+n);
}

