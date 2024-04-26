cmake_minimum_required(VERSION 3.27)
project(task C)

set(CMAKE_C_STANDARD 11)

include_directories(include)

add_executable(task
        src/main.c
        include/eksamen_v24_oppgave2_all_lower.h
        include/eksamen_v24_oppgave2_all_upper.h
        include/eksamen_v24_oppgave2_anagram.h
        include/eksamen_v24_oppgave2_disjoint.h
        include/eksamen_v24_oppgave2_heterogram.h
        include/eksamen_v24_oppgave2_palindrom.h
        include/main.h)
