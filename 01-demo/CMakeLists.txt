cmake_minimum_required(VERSION 3.28)
project(01_demo C)

set(CMAKE_C_STANDARD 11)

include_directories(dir2 hello code student)

add_executable(01_demo main.c
        hello/test.c
        hello/test.h
        dir2/my_dir.h
        dir2/my_dir.c
        code/my_code.h
        code/my_code.c
        student/score.h
        student/score.c)
