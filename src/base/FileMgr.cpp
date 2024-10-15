#include "FileMgr.h"
#include "TTime.h"
#include "StringUtils.h"
#include <sstream>
#include <iostream>

using namespace tmms::base;

//匿名命名空间
//它们只能在其定义的文件中访问，不能被其他文件引用。
//这有助于避免命名冲突，并提高代码的封装性。
namespace{
    //静态变量在程序启动时初始化，并且在程序结束时销毁。
    //它在程序运行期间一直存在，即使没有其他地方引用它.
    static tmms::base::FileLogPtr file_log_nullptr;
}

void FileMgr::OnCheck(){
    int hour_change = 0,day_change = 0,min_change = 0;
    int year = -1,month = -1,day = -1,hour = -1,minute = -1,second = -1;
    TTime::Now(year,month,day,hour,minute,second);

    //last day未定义
    if(last_day_ == -1){
        last_year_ = year;
        last_month_ = month;
        last_day_ = day;
        last_hour_= hour;
        last_minute_ = minute;
        
    }

    //day change
    if(last_day_ != day){
        day_change = 1;
    }

    //hour change
    if(last_hour_ != hour){
        hour_change = 1;
    }

    if(last_minute_ != minute){
        min_change = 1;
    }

    if(!day_change && !hour_change && !min_change){
        //no change
        return;
    }

    std::lock_guard<std::mutex> lk(lock_);

    //遍历
    for(auto log : logs_){
        if(hour_change && log.second->GetRotate() == kRotateHour){
            RotateDuration(log.second,kRotateHour);
        }

        if(day_change && log.second->GetRotate() == kRotateDay){
            RotateDuration(log.second,kRotateDay);
        }

        if(min_change && log.second->GetRotate() == kRotateMinute){
            RotateDuration(log.second,kRotateMinute);
        }
    }

    //对时间进行复制
    /* 
    ？？？为何在此处？？?
    */
    last_year_ = year;
    last_month_ = month;
    last_day_ = day;
    last_hour_= hour;
    last_minute_ = minute;
}

FileLogPtr FileMgr::GetFileLog(const std::string & file_name){
    std::lock_guard<std::mutex> lk(lock_);

    auto iter = logs_.find(file_name);
    if(iter != logs_.end()){
        return iter->second;
    }

    //找不到就创建一个新的
    FileLogPtr log = std::make_shared<FileLog>();

    //文件加载失败
    if(!log->Open(file_name)){
        return file_log_nullptr;
    }

    logs_.emplace(file_name,log);
    return log;
}

void FileMgr::RemoveFileLog(const FileLogPtr &log){
    //只从列表中去掉，并不关闭文件
    std::lock_guard<std::mutex> lk(lock_);
    logs_.erase(log->FileName());
}

//按天或按小时切分
void FileMgr::RotateDuration(const FileLogPtr &file,int type){
    if(file->FileSize() > 0){
        //若文件为空，不需要进行切分

        char ext[128] = {0,};
        switch (type)
        {
        case kRotateDay:
            sprintf(ext,"_%04d-%02d-%02d",last_year_,last_month_,last_day_);
            break;
        case kRotateHour:
            sprintf(ext,"_%04d-%02d-%02dT%02d",last_year_,last_month_,last_day_,last_hour_);
            break;
        case kRotateMinute:
            sprintf(ext,"_%04d-%02d-%02dT%02d-%02d",last_year_,last_month_,last_day_,last_hour_,last_minute_);
            break;
        case kRotateNone:
            return;

        default:
            std::cout << "FileMgr::RotateDaysOrHours type error" << std::endl;
            return;
        }


        //解析源文件
        std::string file_path = StringUtils::FilePath(file->FileName());
        std::string file_name = StringUtils::FileName(file->FileName());
        std::string file_ext = StringUtils::FileExt(file->FileName());
    
        //生成新的文件名
        std::ostringstream new_name;
        new_name << file_path << file_name <<ext<<"."<<file_ext;

        file->Rotate(new_name.str());
    }
}


#define sFileMgr tmms::base::Singleton<tmms::base::FileMgr>::Instance()