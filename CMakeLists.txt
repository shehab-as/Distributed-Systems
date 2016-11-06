cmake_minimum_required(VERSION 3.6)
project(Distributed-Systems)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

set(SOURCE_FILES main.cpp Client/Client.h common/Message/Message.h Server/Server.h common/UDP/UDPSocket.h common/UDP/UDPSocket.cpp common/Message/Message.cpp Client/Client.cpp Server/Server.cpp)
add_executable(untitled ${SOURCE_FILES})
