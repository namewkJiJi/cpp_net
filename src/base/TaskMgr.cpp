#include"TaskMgr.h"
#include"TTime.h"

using namespace tmms::base;

void TaskMgr::OnWork(){
    std::lock_guard<std::mutex> lk(lock_);
    int64_t now = TTime::NowMS();

    //遍历任务集，当前时间大于等于任务时间，则执行任务
    for(auto it = tasks_.begin(); it != tasks_.end();){
        if((*it)->When() <= now){
            (*it)->Run();

            //如果任务的时间没有更新，则删除任务
            if((*it)->When() <= now){
                it = tasks_.erase(it);
                continue;                
            }
        }

        //不管任务是否执行，都继续遍历
        ++it;
    }
}

bool TaskMgr::AddTask(TaskPtr & task)//添加任务
{
    //获取锁，在资源释放时自动释放，无需手动释放
    // std::lock_guard对象被创建时，它将尝试获取传入的互斥锁，
    // 当std::lock_guard对象被销毁时，它将自动释放该锁。
    std::lock_guard<std::mutex> lk(lock_);

    //如果没有找到匹配的元素，返回的迭代器等于容器的end()。
    auto pos = tasks_.find(task);
    if(pos != tasks_.end())//可以将end()换成std::string::npos(from codegeex)
        return false;
    
    tasks_.emplace(task);
    return true;
}

bool TaskMgr::RemoveTask(TaskPtr & task) //移除任务
{
    std::lock_guard<std::mutex> lk(lock_);
    tasks_.erase(task);
    return true;
}