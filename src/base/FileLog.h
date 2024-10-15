#pragma once

#include <string>

namespace tmms{
    namespace base{
        enum RotateType{
            kRotateNone,
            kRotateMinute,
            kRotateHour,
            kRotateDay,

        };

        class FileLog
        {
        public:
            FileLog() = default;
            ~FileLog() = default;

            bool Open(const std::string& file_name);
            size_t Write(const std::string& msg);
            //文件切分
            void Rotate(const std::string &file);
            void SetRotate(RotateType type);
            RotateType GetRotate() const;

            int64_t FileSize() const;
            std::string FileName() const;

        private:
            int fd_{-1};
            std::string file_name_;
            RotateType rotate_type_{kRotateNone};
        };
    }
}