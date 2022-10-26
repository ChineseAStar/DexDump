cmake_minimum_required(VERSION 3.10)
project(dexdump)

set(CMAKE_CXX_STANDARD 14)

# cai：设置libdex中的源码文件，方便编译
aux_source_directory(libdex LIBDEX)
# cai：引入头文件
include_directories(include)
# cai：指定个目标文件
add_executable(${PROJECT_NAME} dexdump/DexDump.cpp ${LIBDEX})
# cai：引用libz库
target_link_libraries(${PROJECT_NAME} z)