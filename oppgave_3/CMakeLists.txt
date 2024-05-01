cmake_minimum_required(VERSION 3.27)
project(task C)

set(CMAKE_C_STANDARD 11)

include_directories(include)

add_executable(task
        src/main.c
        include/main.h
        src/tableReservationList.c
        include/tableReservationList.h
        src/orderList.c
        include/orderList.h
        src/menuApplication.c
        include/menuApplication.h
        src/tableReservationList.c)
