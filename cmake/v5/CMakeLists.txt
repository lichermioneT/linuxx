cmake_minimum_required(VERSION 3.0)
project(test)

file(GLOB SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)     # CMakeLists.txt文件的路径
include_directories(${PROJECT_SOURCE_DIR}/include)


# add_library(calc STATIC ${SRC})                      # 制作库文件的源文件, 静态库
add_library(calc SHARED ${SRC})                      # 制作库文件的源文件, 动态库

 