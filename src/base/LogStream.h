#pragma once

#include "Logger.h"
#include <sstream>

namespace tmms{
    namespace base{
        extern Logger * g_logger;

        class LogStream{
        public:
            LogStream(Logger * logger,const char* file,int line,LogLevel level,const char* func=nullptr);
            ~LogStream();

            template<class T> 
            LogStream& operator<<(const T& value){
                stream_ << value;
                return *this;
            }
        private:
            std::ostringstream stream_;
            Logger * logger_{nullptr};
        };

    }
}

#define LOG_TRACE \
    if(g_logger && tmms::base::g_logger->GetLogLevel() <= tmms::base::kTrace)  \
        tmms::base::LogStream(tmms::base::g_logger,__FILE__,__LINE__,tmms::base::kTrace,__func__)

#define LOG_DEBUG \
    if(g_logger && tmms::base::g_logger->GetLogLevel() <= tmms::base::kDebug)  \
        tmms::base::LogStream(tmms::base::g_logger,__FILE__,__LINE__,tmms::base::kDebug,__func__)

#define LOG_INFO \
    if(g_logger && tmms::base::g_logger->GetLogLevel() <= tmms::base::kInfo)  \
        tmms::base::LogStream(tmms::base::g_logger,__FILE__,__LINE__,tmms::base::kInfo)

#define LOG_WARN \
    tmms::base::LogStream(tmms::base::g_logger,__FILE__,__LINE__,tmms::base::kWarning)

#define LOG_ERR \
    tmms::base::LogStream(tmms::base::g_logger,__FILE__,__LINE__,tmms::base::kError)
