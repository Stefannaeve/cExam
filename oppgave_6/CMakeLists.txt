cmake_minimum_required(VERSION 3.27)
project(oppgave_6 C)

set(CMAKE_C_STANDARD 99)

add_executable(oppgave_6 src/main.c
        include/main.h
        src/oppgave6_test.c
        include/oppgave6_test.h
        src/beautifier.c
        include/beautifier.h
        src/beautifier.c
        include/linkedList.h
        src/linkedList.c

)
