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


# 可以递归搜索文件
file(GLOB SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp) # CMakeLists.txt文件的路径

message("++++++++++++++++++++++++++++++++++")
message(${SRC})
list(REMOVE_ITEM SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)
message("++++++++++++++++++++++++++++++++++")
message(${SRC})
#  头文件
include_directories(${PROJECT_SOURCE_DIR}/include)

# set(LIBRARY_OUTPUT_PATH )  库文件的数据路径

# add_library(calc SHARED ${SRC})C
add_library(calc STATIC ${SRC})

set(tmp hello world)
set(tmp1 ${tmp} ${SRC})

message(${tmp})
message(${tmp1})


list(APPEND tmp "xxx1" "sssss" ${SRC}) # 字符串之间添加分隔符

message(${tmp})


