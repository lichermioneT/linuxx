# 注释
# 注释#[[]] #

# 版本并并不是必须的
cmake_minimum_required(VERSION 3.0)

# 项目的名称随便取
project(test)

# 生成一个可执需要的源文件
# add_executable(calc add.cpp div.cpp main.cpp mul.cpp operations.h sub.cpp)


# cmake命令执行时后面跟随的路径
# aux_source_directory(${PROJECT_SOURCE_DIR}/src SRC) # 可以写路径的 SRC就是变量

message("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx")
# 可以递归搜索文件
file(GLOB SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp) # CMakeLists.txt文件的路径

#  头文件
include_directories(${PROJECT_SOURCE_DIR}/include)

# link_libraries(calc)

link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib2)

add_executable(app ${SRC})

target_link_libraries(app calc)
