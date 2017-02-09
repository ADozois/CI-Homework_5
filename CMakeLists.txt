cmake_minimum_required(VERSION 3.6)
project(CI_Homework_5)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -DNDEBUG -static -std=c99 -pipe")

set(SOURCE_FILES
        LICENSE
        README.md)

add_executable(CI_Homework_5 ${SOURCE_FILES})

