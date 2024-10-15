#pragma once

#include <string>
#include <vector>

namespace tmms{
    namespace base{
        class StringUtils{
            public:
                //判断字符串是否以指定前缀开头
                static bool startsWith(const std::string& str, const std::string& prefix);
                //判断字符串是否以指定后缀结尾
                static bool endsWith(const std::string& str, const std::string& suffix);
                
                //文件操作
                static std::string FilePath(const std::string& path);
                static std::string FileNameExt(const std::string& path);
                static std::string FileName(const std::string& path);
                static std::string FileExt(const std::string& path);


                //字符串分割
                static std::vector<std::string> SplitString(const std::string& str, const std::string& delim);
        
        };
    }
}