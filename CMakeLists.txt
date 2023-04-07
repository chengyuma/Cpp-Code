cmake_minimum_required(VERSION 3.0.0)
project(test_tools VERSION 0.1.0)

include(CTest)
enable_testing()

set(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

add_executable(demo_channel demos/demo_channel.cpp)
add_executable(demo_read_write_lock demos/demo_read_write_lock.cpp)
add_executable(demo_spinlock demos/demo_spinlock.cpp)
add_executable(demo_producer_consumer_model demos/demo_producer_consumer_model.cpp)
add_executable(demo_church_numerals  demos/demo_church_numerals.cpp)
add_executable(demo_lock_free_stack  demos/demo_lock_free_stack.cpp)
add_executable(demo_turnstile  demos/demo_turnstile.cpp)


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
