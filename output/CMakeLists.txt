cmake_minimum_required(VERSION 3.25)
project(ProgettoAPI C)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -g3 -std=gnu11 -O2 -lm")
add_executable(ProgettoAPI main.c prova.c lettura.c bst.c)