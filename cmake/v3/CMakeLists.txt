cmake_minimum_required(VERSION 3.0)
project(test)

# aux_source_directory(${PROJECT_SOURCE_DIR} SRC)

file(GLOB SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp) # CMakeLists.txt文件的路径

 


add_executable(calc ${SRC}) # 取值


