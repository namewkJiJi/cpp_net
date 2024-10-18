# 直播系统

## 3.通用函数

### 设计模式-单例

单例相当于一个全局变量

- 保证一个类只有一个实例
  - 只能自行创建实例
  - 多线程初始化竞争
    - pthread_once
  - 不可复制和移动
    - NonCopyable类
    - 三/五法则，阻止默认合成
- 提供一个访问实例的**全局访问点**

### 在测试TaskMgr时遇到bug

undefined reference to ......

- 编译链接没有问题（cmake文件）
- 函数定义和声明没有问题（.h .cpp）
- 引用没有问题(include)

清空cmake缓存，重新生成

```bash

rm -rf build/*
cd build
cmake ..
make

```

### 使用c++库 jsoncpp

1.利用库里面的amalgamate.py生成dist目录，里面包含cpp和h文件
2.利用cmake

将库拷贝解压到jsoncpp文件夹下

- `cd third_party`
- `mkdir jsoncpp-build` 在文件夹下生成build文件，事后可删除，无用
- `cd jsoncpp-build` 进入该文件夹执行cmake命令
- `cmake "../jsoncpp" -DCMAKE_INSTALL_PREFIX="../../lib/json"` 第一个路径为要编译的文件路径，第二个路径为安装路径
- `make` 编译
- `make install` 安装

jsoncpp 和 jsoncpp-build文件夹可以删除

## 4.高性能网络框架

### 4.1 EventLoop

高性能：

- 吞吐量大
- 延时低
- 资源使用率低

影响性能主要因素：

- 上下文切换
  - 线程切换
  - 系统调用：user mode kernel mode
  - 锁：系统调用/申请锁失败
- 内存拷贝
- 多线程/多进程


流媒体服务器的特点

- 长连接
- 并发量大
- 数据量大
- 平均延时低


网络库：

- 同步IO模型->IO复用模型
- Reactor模式->事件循环都在一个工作线程中
- 线程池->每个线程是**一个**单独的事件循环
  - 减少线程切换
  - 减少锁
- 减少IO调用
  - 分散读，聚集写
- 减少内存拷贝
  - 发送队列只保存地址和长度


事件循环的功能：

- IO就绪事件监听
- 处理
- 管理

4-9 EventLoop-任务队列 完成