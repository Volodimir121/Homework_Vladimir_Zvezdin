cmake_minimum_required(VERSION 3.10)
project(Practice1_ForwardList C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

if (MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -pedantic)
endif()

add_executable(practice1
    list.h
    list.c
    main.c
)
