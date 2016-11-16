cmake_minimum_required(VERSION 3.6)
project(Distributed-Systems)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

set(CLIENT_SOURCE_FILES Client/Client.h common/Message/Message.h common/UDP/UDPSocket.h common/UDP/UDPSocket.cpp common/Message/Message.cpp Client/Client.cpp Client/main.cpp common/registry_rpc.h common/peer_rpc.h common/CM/CM.cpp common/CM/CM.h)
add_executable(client ${CLIENT_SOURCE_FILES})

set(REGISTRY_SOURCE_FILES common/Message/Message.h common/UDP/UDPSocket.h common/UDP/UDPSocket.cpp common/Message/Message.cpp common/registry_rpc.h common/peer_rpc.h common/CM/CM.cpp common/CM/CM.h Registry/Registry.cpp Registry/Registry.h)
add_executable(registry ${REGISTRY_SOURCE_FILES})
