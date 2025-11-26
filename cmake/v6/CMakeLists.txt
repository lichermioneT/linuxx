cmake_minimum_required(VERSION 3.0)
project(test)

file(GLOB SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)    # CMakeLists.txt文件的路径  main.cpp
include_directories(${PROJECT_SOURCE_DIR}/include)  # 头文件

link_libraries(calc)                                # 静态库的名称
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib1)  # 静态库的路径

add_executable(app ${SRC})                          # 静态库会打包到可执行 程序里面的

