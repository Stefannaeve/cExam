cmake_minimum_required(VERSION 3.27)
project(task C)

set(CMAKE_C_STANDARD 11)

include_directories(include)

add_executable(task
        src/eksamen_v24_oppgave2_all_lower.c
        src/eksamen_v24_oppgave2_all_upper.c
        src/eksamen_v24_oppgave2_anagram.c
        src/eksamen_v24_oppgave2_disjoint.c
        src/eksamen_v24_oppgave2_heterogram.c
        src/eksamen_v24_oppgave2_palindrom.c
        include/eksamen_v24_oppgave2_all_lower.h
        include/eksamen_v24_oppgave2_all_upper.h
        include/eksamen_v24_oppgave2_anagram.h
        include/eksamen_v24_oppgave2_disjoint.h
        include/eksamen_v24_oppgave2_heterogram.h
        include/eksamen_v24_oppgave2_palindrom.h
        include/main.h
        src/main.c
        include/program.h
        src/program.c
        src/program.c)
