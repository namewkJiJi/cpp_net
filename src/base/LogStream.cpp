#include "LogStream.h"
#include <cstring>
#include "TTime.h"
#include<unistd.h>
#include<sys/syscall.h>
#include <string>
#include <iostream>


using namespace tmms::base;

//线程id
static thread_local pid_t thread_id = 0;
Logger * tmms::base::g_logger = nullptr;


//将loglevel转化为字符串
const char* log_string[] = {
    "TRACE",
    "DEBUG",
    "INFO",
    "WARN",
    "ERROR",
};

LogStream::LogStream(Logger * logger,const char* file,int line,LogLevel level,const char* func)
:logger_(logger)
{
    //查找文件名（c风格）
    //最后一个斜杠'/'的位置
    const char *filename = strrchr(file,'/');
    if(filename){
        filename += 1;//取斜杠后面的
    }else{
        filename = file;
    }

    //先输出时间
    this->stream_ << TTime::ISOTime();

    //输出线程id
    if(thread_id == 0){
        //只初始化一次
        thread_id = static_cast<pid_t>(::syscall(SYS_gettid));
    }

    stream_ << thread_id;
    stream_ << log_string[level];
    stream_ << '[' << filename << ':' << line << ']';
    if(func){
        stream_ << "[" << func << "]";
    }
}

LogStream::~LogStream(){
    stream_ << '\n';
    if(logger_){
        logger_->Write(stream_.str());
    }else{
        std::cout<< stream_.str() <<std::endl;
    }
}

