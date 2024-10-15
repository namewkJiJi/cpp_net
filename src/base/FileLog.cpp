#include"FileLog.h"
#include<fcntl.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<iostream>
#include"StringUtils.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <cstring>
#include <cerrno>


using namespace tmms::base;

bool FileLog::Open(const std::string& file_name){
    file_name_ = file_name;

    // 确保目录存在
    // std::string file_path = StringUtils::FilePath(file_name);
    // if(mkdir(file_path.c_str(), DEFFILEMODE) == -1){
    //     perror("mkdir");
    //     return false;
    // }

    int fd = ::open(file_name.c_str(), O_CREAT | O_WRONLY | O_APPEND, 0644);
    if(fd < 0){
        std::cerr << "Error opening file: " << std::strerror(errno) << std::endl;
        return false;
    }
    fd_ = fd;
    return true;
}

size_t FileLog::Write(const std::string& msg){
    int fd = fd_ == -1 ? 1: fd_;//fd为-1时，输出到标准文件（fd设为一）
    return ::write(fd, msg.data(), msg.size());
}

//文件切分
void FileLog::Rotate(const std::string &file){
    if(file_name_.empty()){
        return;
    }

    int ret = ::rename(file_name_.c_str(), file.c_str());
    if(ret != 0){
        std::cerr<<"rename failed";
        return;
    }

    //打开新文件
    int fd = ::open(file.c_str(), O_CREAT | O_WRONLY | O_APPEND, DEFFILEMODE);
    if(fd < 0){
        return;
    }

    //如果 fd_ 已经指向某个文件描述符，则先关闭 fd_
    //将 fd 指向的文件描述符复制到 fd_
    //使得 fd_ 和 fd 指向同一个文件
    ::dup2(fd,fd_);

    //dup2 使得 fd_ 和 fd 引用同一个文件
    //但它们是两个独立的文件描述符，所以关闭 fd 不会影响 fd_
    ::close(fd);
}

void FileLog::SetRotate(RotateType type){
    rotate_type_ = type;
}

RotateType FileLog::GetRotate() const{
    return rotate_type_;
}

int64_t FileLog::FileSize() const{
    return ::lseek64(fd_, 0, SEEK_END);
}

std::string FileLog::FileName() const{
    return file_name_;
}