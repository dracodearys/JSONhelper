cmake_minimum_required(VERSION 2.6 )
project(json_helper)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall")
set(CMAKE_CXX_FLAGS   "-std=c++14")             # c++1z
set(CMAKE_CXX_FLAGS   "-g")                     # 调试信息
set(CMAKE_CXX_FLAGS   "-Wall")                  # 开启所有警告
SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
include_directories(../src)

add_library(json ../src/json.cpp)
add_library(parse ../src/parse.cpp)

enable_testing()
add_executable(Test test.cpp)
target_link_libraries(Test json parse /usr/lib/libgtest.so /usr/lib/libgtest_main.so -pthread)
add_test(NAME Test COMMAND Test)

add_executable(jsonchecker jsonchecker.cpp)
target_link_libraries(jsonchecker json parse)
