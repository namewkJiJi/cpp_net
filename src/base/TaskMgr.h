#pragma once

//任务管理器，全局唯一

#include "Task.h"
#include "Singleton.h"
#include<unordered_set>
#include<mutex>

namespace tmms{
    namespace base{
        class TaskMgr: public NonCopyable
        {
        public:
            TaskMgr() = default;
            ~TaskMgr() = default;

            void OnWork();
            bool AddTask(TaskPtr & task); //添加任务
            bool RemoveTask(TaskPtr & task); //移除任务
        private:
            std::unordered_set<TaskPtr> tasks_;
            std::mutex lock_;
        };
    }
    //定义一个单例的TaskMgr
    #define sTaskMgr tmms::base::Singleton<tmms::base::TaskMgr>::Instance()

}