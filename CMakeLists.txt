cmake_minimum_required(VERSION 3.20)
project(Ccomponent C)

set(CMAKE_C_STANDARD 99)

add_executable(Ccomponent main.c lib/number.h lib/number.c lib/calculate.c lib/calculate.h lib/tools.c lib/tools.h)
