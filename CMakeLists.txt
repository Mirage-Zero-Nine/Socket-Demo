cmake_minimum_required(VERSION 3.10)
project(Socket_Demo C)

set(CMAKE_C_STANDARD 99)

add_executable(Socket_Demo Client.c Main_Server.c Server_A.c Server_C.c Server_B.c)