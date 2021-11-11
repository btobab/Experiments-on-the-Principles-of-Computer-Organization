cmake_minimum_required(VERSION 3.20)
project(Ccomponent C)

set(CMAKE_C_STANDARD 99)

add_executable(Ccomponent main.c number.h number.c calculate.c calculate.h)
