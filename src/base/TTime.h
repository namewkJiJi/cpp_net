#pragma once
#include <cstdint>
#include <string>
namespace tmms
{
    namespace base
    { 
        class TTime
        {
        public:
            static int64_t NowMS();//表示当前的UTC时间，单位是毫秒
            static int64_t Now();//表示当前的UTC时间，单位是秒
            //表示当前时间，返回当前的年月日时分
            static int64_t Now(int &year,int &month,int&day,int &hour,int&minute,int&second);
            //表示返回当前时间ISO格式的字符串(yyyy-MM-ddTHH:mm:ss)
            static std::string ISOTime();
        };
    }
}