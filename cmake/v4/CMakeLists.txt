cmake_minimum_required(VERSION 3.0)
project(test)


file(GLOB SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)     # CMakeLists.txt文件的路径
 
include_directories(${PROJECT_SOURCE_DIR}/include)       # 指定头文件的路径


add_executable(calc ${SRC}) # 取值

