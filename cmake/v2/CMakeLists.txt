cmake_minimum_required(VERSION 3.0)
project(test)
set(SCR_LIST add.cpp div.cpp main.cpp mul.cpp  sub.cpp)  # cmake一切皆是字符串
set(CMAKE_CXX_STANDARD 11)
set(EXECUTABLE_OUTPUT_PATH /home/lic/Desktop/cmake/v2)   # 输出路径，没有会自动生成的 
add_executable(calc ${SCR_LIST})                         # 取变量 ${}



