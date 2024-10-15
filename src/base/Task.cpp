#include"Task.h"
#include"TTime.h"

using namespace tmms::base;

// 构造函数，接受一个TaskCallback类型的回调函数和一个int64_t类型的间隔时间
Task::Task(const TaskCallback &cb,int64_t interval)
:interval_(interval),when_(TTime::NowMS() + interval),cb_(cb)
{}

// 构造函数，接受一个TaskCallback类型的右值引用回调函数和一个int64_t类型的间隔时间
Task::Task(const TaskCallback &&cb,int64_t interval)
:interval_(interval),when_(TTime::NowMS() + interval),cb_(std::move(cb))
{}

// 运行函数，如果回调函数存在，则调用回调函数
void Task::Run(){
    if(cb_){
        cb_(shared_from_this());
    }
}

// 重启函数，将when_设置为当前时间加上间隔时间
void Task::Restart(){
    when_ = TTime::NowMS() + interval_;
}