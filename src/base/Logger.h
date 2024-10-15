#pragma once

#include "FileMgr.h"
#include "NonCopyable.h"
#include <string>  

namespace tmms{
    namespace base{
        //对日志级别进行定义
        enum LogLevel{
            kTrace,
            kDebug,
            kInfo,
            kWarning,
            kError,
            kMaxNumOfLogLevels,
        };

        class Logger: public NonCopyable
        {
        public:
            Logger(const FileLogPtr & log);
            ~Logger() = default;
            
            void SetLogLevel(const LogLevel & level);
            LogLevel GetLogLevel() const;
            void Write(const std::string &msg);
        private:
            LogLevel level_{kDebug};

            FileLogPtr log_;
        };

    }   
}