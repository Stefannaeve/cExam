cmake_minimum_required(VERSION 3.27)
project(oppgave_6 C)

set(CMAKE_C_STANDARD 99)

add_executable(oppgave_6 src/main.c
        include/main.h
        src/newBeautifier.c
        src/oppgave_6.c
        include/oppgave_6.h
        src/newNewBeautifier.c
        include/newNewBeautifier.h
        src/newNewBeautifier.c
        include/linkedList.h
        src/linkedList.c

)
