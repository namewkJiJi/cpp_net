#pragma once

#include "NonCopyable.h"
#include <pthread.h>

// 单例
namespace tmms{
    namespace base{
        template <typename T>
        class Singleton : public NonCopyable
        {
        public:
        //不允许构造和析构
            Singleton() = delete;
            ~Singleton() = delete;

            static T*& Instance(){ // 返回一个指向单例对象的指针
                //确保只初始化一次，init为静态函数，可以取地址
                pthread_once(&ponce_,&Singleton::init);
                return value_;           
            }
        private:
            static void init(){
                if(value_ == nullptr){
                    value_ = new T();
                }
            }
            //一定要是静态的，只有静态的才有函数的地址，然后将函数的地址当作参数传递

            static pthread_once_t ponce_;
            static T* value_;        
        };


        //在类外初始化静态成员变量
        template <typename T>
        pthread_once_t Singleton<T>::ponce_ = PTHREAD_ONCE_INIT;

        template <typename T>
        T* Singleton<T>::value_ = nullptr;
    }
}