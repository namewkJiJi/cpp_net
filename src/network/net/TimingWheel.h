#pragma once

#include <memory>
#include <functional>
#include <cstdint>
#include <deque>
#include <unordered_set>
#include <vector>


namespace tmms{
    namespace network{
        using Func = std::function<void()>;

        using EntryPtr = std::shared_ptr<void>;
        using WheelEntry = std::unordered_set<EntryPtr>;
        using Wheel = std::deque<WheelEntry>;
        using Wheels = std::vector<Wheel>;

        enum TimingType{
            kTimingTypeSecond = 0,
            kTimingTypeMinute,
            kTimingTypeHour,
            kTimingTypeDay
        };

        //有多少秒？
        const int kTimingMinute = 60;
        const int kTimingHour = 60 * kTimingMinute;
        const int kTimingDay = 24 * kTimingHour;

        class CallbackEntry{
        public:
            CallbackEntry(const Func & cb)//cb = callback
            :cb_(cb){}

            ~CallbackEntry(){
                if(cb_){
                    cb_();
                }
            }
        private:
            Func cb_;
        };  
        using CallbackEntryPtr = std::shared_ptr<CallbackEntry>;

        class TimingWheel{
        public:
            TimingWheel();
            ~TimingWheel();

            void InsertEntry(uint32_t delay,EntryPtr entryPtr);
            void OnTimer(int64_t now);
            void PopUp(Wheel & bq);
            void RunAfter(double delay,const Func & cb);
            void RunAfter(double delay,Func && cb);
            void RunEvery(double interval, const Func & cb);
            void RunEvery(double interval, Func && cb);

        private:
            void InsertSecondEntry(uint32_t delay,EntryPtr entryPtr);
            void InsertMinuteEntry(uint32_t delay,EntryPtr entryPtr);
            void InsertHourEntry(uint32_t delay,EntryPtr entryPtr);
            void InsertDayEntry(uint32_t delay,EntryPtr entryPtr);
            
            Wheels wheels_;
            int64_t last_ts_{0};
            uint64_t tick_{0};
        };
    }
}