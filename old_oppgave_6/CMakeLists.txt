cmake_minimum_required(VERSION 3.27)
project(task C)

set(CMAKE_C_STANDARD 11)

include_directories(include)

add_executable(task
        src/beautifier.c
        src/main.c
        src/newBeautifier.c
        src/oppgave_6.c
        include/beautifier.h
        include/main.h
        include/newBeautifier.h
        include/oppgave_6.h
)
