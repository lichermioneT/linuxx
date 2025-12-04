cmake_minimum_required(VERSION 3.0)
project(test)

file(GLOB SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)     # CMakeLists.txt文件的路径
message(${SRC})
message("================")
list(REMOVE_ITEM SRC ${PROJECT_SOURCE_DIR}/src/main.cpp)
message(${SRC})

include_directories(${PROJECT_SOURCE_DIR}/include)


# add_library(calc STATIC ${SRC})                      # 制作库文件的源文件, 静态库
add_library(calc SHARED ${SRC})                      # 制作库文件的源文件, 动态库

# set(tmp hello lic)
# set(tmp2 ${tmp} ${SRC})                # 追加字符串
# message(${tmp})
# message(${tmp2})

# list(APPEND tmp "xxx" "sss" ${SRC})             # 追加字符串
# message(${tmp})

# abc:abc123:xxx1 分隔符
