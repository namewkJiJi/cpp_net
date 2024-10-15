#pragma once

//定时任务类

#include<cstdint>
#include<functional>
#include<memory>


namespace tmms{
    namespace base{
        class Task;
        //using：创建类型别名
        using TaskPtr = std::shared_ptr<Task>;
        //std::shared_ptr是一种智能指针，用于自动管理动态分配的内存
        //当最后一个shared_ptr被销毁时，它所指向的对象也会被自动删除。
        using TaskCallback = std::function<void(const TaskPtr &)>;
        //接受一个const TaskPtr &类型的参数，并返回void
        //std::function是C++标准库中的一个类模板，可以存储、复制和调用任何可调用目标

        class Task : public std::enable_shared_from_this<Task>
        {
        public:
            Task(const TaskCallback &cb,int64_t interval);
            Task(const TaskCallback &&cb,int64_t interval);

            void Run();
            void Restart();
            int64_t When() const{
                return when_;
            }
        private:
        //列表初始化，和interal_ = 0;效果相同
            int64_t interval_{0};
            int64_t when_{0};
            TaskCallback cb_;
        };
    }
}