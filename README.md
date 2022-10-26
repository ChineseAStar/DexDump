# 一、运行
## cmake
```bash
mkdir build
cd build/
cmake ..
make
```
## g++
```bash
 g++ -o test -Iinclude dexdump/DexDump.cpp libdex/*.cpp -lz -w
```
# 二、迁移
## 2.1 目录结构

```
.
├── dexdump
├── include
│ ├── libdex
│ ├── utils
│ └── vm
└── libdex
```
## 2.2 AOSP
* 将AOSP中的`/dalvik/dexdump`中的`dexdump`和`libdex`目录拷贝过来。
  * 对应目录`dexdump`和`libdex`。
* 然后将`libdex`中的所有`.h`复制到`include/libdex`目录中。
  * 统一头文件存放位置。
## 2.3 参考github
地址：https://github.com/xia0pin9/dexdump
* 拉取上述地址的项目，将其中的`utils`和`vm`目录复制到`include`目录中。
  * 对应`include/utils`和`include/vm`目录。
* 复制项目中`libdex`目录中的`ZipArchive.cpp`和`CmdUtils.cpp`文件，替换原`libdex`中的对应文件
  * 主要是新版的`zip`相关代码改动太大，其中`ZipArchive.cpp`可能没有源文件对应，直接放到`libdex`中即可。
* 复制项目中的`libdex/ZipArchive.h`文件替换原文件
  * `libdex`和`include/libdex`中的都替换一下
## 2.4 修改引用
* github项目中，使用`../`方式引用部分头文件，统一去掉`../`
  * 通过`cmake`保证引用正确
* 将`libdex/DexFile.h`中的`#include <log/log.h>`替换成`#include "vm/Common.h"`

