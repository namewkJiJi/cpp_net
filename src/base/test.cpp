// #include"Singleton.h"
// #include <iostream>

// using namespace tmms::base;

// class A :public NonCopyable
// {
// public:
//     A() = default;
//     ~A() = default;

//     void Print(){
//         std::cout << "A" << std::endl;
//     };

// };

// int main(){
//     auto a = Singleton<A>::Instance();

//     a->Print();

//     return 0;
// }

// #include <iostream>
// #include <cassert>
// #include <vector>
// #include <string>
// #include "StringUtils.h"

// using namespace tmms::base;

// int main() {

//     // Test case 1: Check if a string starts with a specified prefix
//     assert(StringUtils::startsWith("HelloWorld", "Hello") == true);
//     assert(StringUtils::startsWith("HelloWorld", "World") == false);

//     // // Test case 2: Check if a string ends with a specified suffix
//     // assert(StringUtils::endsWith("HelloWorld", "World") == true);
//     // assert(StringUtils::endsWith("HelloWorld", "Hello") == false);

//     // Test case 3: File path operations - get file path
//     assert(StringUtils::FilePath("C:/Users/username/Documents/file.txt") == "C:/Users/username/Documents");
//     // std::cout<<StringUtils::FilePath("C:/Users/username/Documents/file.txt")<<std::endl;

//     // Test case 4: File path operations - get file name with extension
//     assert(StringUtils::FileNameExt("C:/Users/username/Documents/file.txt") == "file.txt");

//     // Test case 5: File path operations - get file name without extension
//     assert(StringUtils::FileName("C:/Users/username/Documents/file.txt") == "file");

//     // Test case 6: File path operations - get file extension
//     assert(StringUtils::FileExt("C:/Users/username/Documents/file.txt") == "txt");

//     // Test case 7: String split operation
//     std::vector<std::string> result = StringUtils::SplitString("Hello,World,How,Are,You", ",");
//     assert(result.size() == 5);
//     assert(result[0] == "Hello");
//     assert(result[1] == "World");
//     assert(result[2] == "How");
//     assert(result[3] == "Are");
//     assert(result[4] == "You");

//     printf("All test cases pass\n");
//     return 0;
// }

// #include <iostream>
// #include <cassert>
// #include <ctime>
// #include "TTime.h"

// using namespace tmms::base;

// int main() {
//     // Test case 1: Test the NowMS function
//     int64_t nowMS = TTime::NowMS();
//     printf("NowMS: %ld\n", nowMS);
//     assert(nowMS >= 0);

//     // Test case 2: Test the Now function
//     int64_t now = TTime::Now();
//     printf("Now: %ld\n", now);
//     assert(now >= 0);

//     // Test case 3: Test the Now function with year, month, day, hour, minute, and second
//     int year, month, day, hour, minute, second;
//     TTime::Now(year, month, day, hour, minute, second);
//     printf("Now: %d-%02d-%02d %02d:%02d:%02d\n", year, month, day, hour, minute, second);
//     assert(year >= 0);
//     assert(month >= 1 && month <= 12);
//     assert(day >= 1 && day <= 31);
//     assert(hour >= 0 && hour <= 23);
//     assert(minute >= 0 && minute <= 59);
//     assert(second >= 0 && second <= 59);

//     // Test case 4: Test the ISOTime function
//     std::string isoTime = TTime::ISOTime();
//     printf("ISOTime: %s\n", isoTime.c_str());
//     assert(isoTime.length() == 19);
//     assert(isoTime[4] == '-' && isoTime[7] == '-' && isoTime[10] == 'T' && isoTime[13] == ':' && isoTime[16] == ':');

//     printf("All test cases pass\n");
//     return 0;
// }

// #include "TaskMgr.h"
// #include "Task.h"
// #include "TTime.h"
// #include <iostream>
// #include <thread>
// #include <chrono>

// using namespace tmms::base;

// void testTask(){
//     TaskPtr task1 = std::make_shared<Task>(
//         [](const TaskPtr& task){
//             std::cout << "Task 1 executed" <<TTime::NowMS() <<std::endl;
//         },
//         1000
//     );

//     TaskPtr task2 = std::make_shared<Task>(
//         [](const TaskPtr& task){
//             std::cout << "Task 2 executed" <<TTime::NowMS() <<std::endl;
//             task->Restart();
//         },
//         1000
//     );

//     TaskPtr task3 = std::make_shared<Task>(
//         [](const TaskPtr& task){
//             std::cout << "Task 3 executed" <<TTime::NowMS() <<std::endl;
//             task->Restart();
//         },
//         500
//     );

//     TaskPtr task4 = std::make_shared<Task>(
//         [](const TaskPtr& task){
//             std::cout << "Task 4 executed" <<TTime::NowMS() <<std::endl;
//         },
//         2000
//     );

//     sTaskMgr->AddTask(task1);
//     sTaskMgr->AddTask(task2);
//     sTaskMgr->AddTask(task3);
//     sTaskMgr->AddTask(task4);

// }

// int main() {
//     testTask();

//     while(1){
//         sTaskMgr->OnWork();
//         std::this_thread::sleep_for(std::chrono::milliseconds(50));
//     }

//     return 0;
// }   

// #include "LogStream.h"

// using namespace tmms::base;

// void test(){
//     LOG_TRACE<<"test trace!!!";
//     LOG_DEBUG<<"test debug!!!";
//     LOG_INFO<<"test info!!!";
//     LOG_WARN<<"test warn!!!";
//     LOG_ERR<<"test error!!!";
// }

// int main() {
//     tmms::base::g_logger = new Logger();
//     tmms::base::g_logger->SetLogLevel(kError);
//     test();

//     return 0;
// }



#include "LogStream.h"
#include <thread>
#include <chrono>
#include "FileMgr.h"
#include "FileLog.h"
#include "TTime.h"
#include "TaskMgr.h"

using namespace tmms::base;
std::thread t;
void test(){
    t = std::thread([](){
        while(true){
            LOG_DEBUG<<"test debug!!!now:"<<TTime::ISOTime();
            LOG_INFO<<"test info!!!now:"<<TTime::ISOTime();
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    });
}

int main() {
    FileLogPtr log = sFileMgr->GetFileLog("test.log");
    log->SetRotate(kRotateMinute);
    tmms::base::g_logger = new Logger(log);
    tmms::base::g_logger->SetLogLevel(kTrace);

    TaskPtr task = std::make_shared<Task>([](const TaskPtr& task){
        sFileMgr->OnCheck();
        task->Restart();
    },
    1000);

    sTaskMgr->AddTask(task);
    test();

    while(true){
        sTaskMgr->OnWork();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return 0;
}
