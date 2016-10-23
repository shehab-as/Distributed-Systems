cmake_minimum_required(VERSION 3.6)
project(Distributed-Systems)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp Client.h Message.h Server.h UDP/UDPClientSocket.h UDP/UDPServerSocket.h UDP/UDPSocket.h UDP/UDPSocket.cpp)
add_executable(untitled ${SOURCE_FILES})