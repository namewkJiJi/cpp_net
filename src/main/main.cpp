#include<stdio.h>
#include "Config.h"
#include "LogStream.h"
#include "FileMgr.h"
#include "TaskMgr.h"
#include "Logger.h"
#include <iostream>
#include <thread>


using namespace tmms::base;


int main(){
    if(!sConfigMgr->LoadConfig("../config/config.json")){
        std::cerr << "Failed to load config file"<< std::endl;
        return -1;
    }

    ConfigPtr config = sConfigMgr->GetConfig();
    LogInfoPtr log_info = config->GetLogInfo();
    std::cout<<"Log Level: "<<log_info->level
            <<", Log name: "<<log_info->name
            <<", Log path: "<<log_info->path
            <<", Log rotate type: "<<log_info->rotate_type
            <<std::endl;

    FileLogPtr log = sFileMgr->GetFileLog(log_info->path + log_info->name);
    if(log){
        log->SetRotate(log_info->rotate_type);
        g_logger = new Logger(log);
    }else{
        std::cerr << "Failed to create log file"<< std::endl;
        return -1;
    }

    g_logger->SetLogLevel(log_info->level);

    TaskPtr task = std::make_shared<Task>(
        [](const TaskPtr &task){
            sFileMgr->OnCheck();
            task->Restart();
        },
        1000
    );
    sTaskMgr->AddTask(task);

    while(1){
        sTaskMgr->OnWork();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    
    
    return 0;
}