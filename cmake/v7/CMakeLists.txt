
cmake_minimum_required(VERSION 3.0)


project(test)


file(GLOB SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp) # CMakeLists.txt文件的路径
include_directories(${PROJECT_SOURCE_DIR}/include)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib2)

add_executable(app ${SRC})
target_link_libraries(app calc)
