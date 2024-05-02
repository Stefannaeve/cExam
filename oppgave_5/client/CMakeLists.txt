cmake_minimum_required(VERSION 3.27)
project(client C)

set(CMAKE_C_STANDARD 99)

add_executable(client src/main.c
        src/client.c
        src/client.h
        src/main.h
)
