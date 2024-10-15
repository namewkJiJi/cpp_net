# 直播系统

## 3。通用函数

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

### file access error

./bin/log -- Permission denied
cd ./log -- Permission denied

