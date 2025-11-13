cmake_minimum_required(VERSION 2.8)
project(TestProject)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置可执行文件输出目录为源文件目录（当前文件夹）
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

# 创建可执行文件
add_executable(c++11_app c++11.cpp)