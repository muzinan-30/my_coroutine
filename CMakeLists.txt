cmake_minimum_required(VERSION 3.26)
project(My_coroutine)

set(CMAKE_CXX_STANDARD 17)

add_executable(My_coroutine main.cpp
        test/ucontext_demo.cpp
        src/Fiber.cpp
        src/Fiber.h
        src/simple_fiber_scheduler.cpp)
