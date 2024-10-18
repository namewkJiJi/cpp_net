#pragma once

#include "json/json.h"
#include "NonCopyable.h"
#include "Singleton.h"
#include "Logger.h"
#include "FileLog.h"
#include <string>
#include <cstdint>
#include <memory>
#include <mutex>

namespace tmms{
    namespace base{

        // 日志信息
        struct LogInfo{
            LogLevel level;//日志级别
            std::string path;//日志文件所在文件夹的路径
            std::string name;//日志文件名
            RotateType rotate_type{kRotateNone};//日志切分类型
        };
        
        using LogInfoPtr = std::shared_ptr<LogInfo>;

        class Config{
        public:
            Config() = default;
            ~Config() = default;

            bool LoadConfig(const std::string &file);
            bool ParseLogInfo(const Json::Value & root);
            LogInfoPtr& GetLogInfo();

            std::string name_;
            int32_t cpu_start_{0};
            int32_t thread_nums_{1};

        private:
            LogInfoPtr log_info_;
        };

        using ConfigPtr = std::shared_ptr<Config>;

        class ConfigMgr:public NonCopyable{
        public:
            ConfigMgr() = default;
            ~ConfigMgr() = default;

            bool LoadConfig(const std::string & file);
            ConfigPtr& GetConfig();
        
        private:
            ConfigPtr config_;
            std::mutex lock_;
            //智能指针线程不安全
        };

        #define sConfigMgr tmms::base::Singleton<tmms::base::ConfigMgr>::Instance()
    }
}