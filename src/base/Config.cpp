#include "Config.h"
#include "LogStream.h"
#include <fstream>
#include <iostream>

using namespace tmms::base;

bool Config::LoadConfig(const std::string &file){
    LOG_DEBUG << "config file: "<<file;
    Json::Value root;
    Json::CharReaderBuilder reader;
    std::ifstream in(file);
    std::string errs;

    auto ok = Json::parseFromStream(reader,in,&root,&errs);

    if(!ok){
        LOG_ERR << "config file: "<< file << " parse error: "<<errs;
    
        return false;
    }

    Json::Value nameObj = root["name"];
    if(!nameObj.isNull()){
        name_ = nameObj.asString();
    }

    Json::Value cpu_startObj = root["cpu_start"];
    if(!cpu_startObj.isNull()){
        cpu_start_ = cpu_startObj.asInt();
    }

    Json::Value threadObj = root["thread"];
    if(!threadObj.isNull()){
        thread_nums_ = threadObj.asInt();
    }

    Json::Value logObj = root["log"];
    if(!logObj.isNull()){
        ParseLogInfo(logObj);
    }

    return true;
}

bool Config::ParseLogInfo(const Json::Value & root){
    //分配内存
    log_info_ = std::make_shared<LogInfo>();
    
    Json::Value levelObj = root["level"];
    if(levelObj.isNull()){
        return false;
    }
    std::string level = levelObj.asString();
    if(level == "TRACE"){
        log_info_->level = kTrace;
    }
    else if(level == "DEBUG"){
        log_info_->level = kDebug;
    }
    else if(level == "INFO"){
        log_info_->level = kInfo;
    }
    else if(level == "WARN"){
        log_info_->level = kWarning;
    }
    else if(level == "ERROR"){
        log_info_->level = kError;
    }else{
        return false;
    }

    Json::Value nameObj = root["name"];
    if(nameObj.isNull()){
        return false;
    }
    log_info_->name = nameObj.asString();


    Json::Value pathObj = root["path"];
    if(pathObj.isNull()){
        return false;
    }
    log_info_->path = pathObj.asString();


    Json::Value rtObj = root["rotate"];
    if(rtObj.isNull()){
        return false;
    }
    std::string rt = rtObj.asString();
    if(rt == "DAY"){
        log_info_->rotate_type = kRotateDay;
    }
    else if(rt == "HOUR"){
        log_info_->rotate_type = kRotateHour;
    }


    return true;
}

LogInfoPtr& Config::GetLogInfo(){
    return log_info_;
}


bool ConfigMgr::LoadConfig(const std::string & file){
    LOG_DEBUG << "load config file: "<<file;
    ConfigPtr config = std::make_shared<Config>();
    
    if(config->LoadConfig(file)){
        std::lock_guard<std::mutex> lk(lock_);
        config_ = config;
        return true;
    }
    return false;
}   


ConfigPtr& ConfigMgr::GetConfig(){
    std::lock_guard<std::mutex> lk(lock_);
    return config_;
}
 
