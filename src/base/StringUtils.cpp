#include "StringUtils.h"

using namespace tmms::base;

//判断字符串是否以指定前缀开头
bool StringUtils::startsWith(const std::string& str, const std::string& prefix){
    
    if(prefix.empty()){
        return true;
    }

    if(str.empty()){
        return false;
    }

    auto strlen = str.length();
    auto prefixlen = prefix.length();
    if(strlen < prefixlen){
        return false;
    }

    return str.compare(0, prefixlen, prefix) == 0;
}


//判断字符串是否以指定后缀结尾
bool StringUtils::endsWith(const std::string& str, const std::string& suffix){
    return false;
}

//获取文件路径
std::string StringUtils::FilePath(const std::string& path){
    auto pos = path.find_last_of("/\\");
    if(pos != std::string::npos){
        return path.substr(0, pos+1);//包括最后一个 /
    }else{
        return "./";
    }
}

//获取文件名及后缀
std::string StringUtils::FileNameExt(const std::string& path){
    // 与指定字符串（或字符）中的某个字符匹配的字符
    auto pos = path.find_last_of("/\\");
    if(pos != std::string::npos){
        if(pos < path.size() - 1){
            return path.substr(pos + 1);
        }
    }

    return path;
}

//获取文件名
std::string StringUtils::FileName(const std::string& path){
    std::string filename = FileNameExt(path);
    auto pos = filename.find_last_of(".");
    if(pos != std::string::npos){
        if(pos != 0){
            return filename.substr(0, pos);
        }
    }

    return filename;
}

std::string StringUtils::FileExt(const std::string& path){
    std::string filename = FileNameExt(path);
    auto pos = filename.find_last_of(".");
    if(pos != std::string::npos){
        if((pos != 0) && (pos < filename.size() - 1)){
            return filename.substr(pos + 1);
        }
    }

    return std::string();
}

//字符串分割
std::vector<std::string> StringUtils::SplitString(const std::string& str, const std::string& delim){
    if(delim.empty()){
        return std::vector<std::string>{};
    }

    //0.123.456.
    std::vector<std::string> result;
    size_t last = 0;
    size_t pos = 0;
    //从b字符串中查找a字符串 find 和 find_last_of不同
    while((pos = str.find(delim, last)) != std::string::npos){
        //当分隔符在字符串开头时,pos=last=0,会像vector中添加一个空字符串
        //不要要添加空字符串
        if(pos > last){
            result.emplace_back(str.substr(last, pos - last));
            //push_back() 在向 vector 尾部添加一个元素时，首先会创建一个临时对象，然后再将这个临时对象移动或拷贝到 vector 中（如果是拷贝的话，事后会自动销毁先前创建的这个临时元素）；而 emplace_back() 在实现时，则是直接在 vector 尾部创建这个元素，省去了移动或者拷贝元素的过程。
        }

        last = pos + delim.size();
        //如果分隔符在末尾，last指针超出字符串长度，则退出循环
        if(last >= str.size()){
            return result;
        }
    }

    //不要忘记最后一个分割符的后部分
    if(last < str.size()){
        result.emplace_back(str.substr(last));
    }

    return result;
}