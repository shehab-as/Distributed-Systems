cmake_minimum_required(VERSION 3.6)
project(Distributed-Systems)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

set(CLIENT_SOURCE_FILES  common/Message/Message.h common/UDP/UDPSocket.h common/UDP/UDPSocket.cpp common/Message/Message.cpp  common/registry_rpc.h common/peer_rpc.h common/CM/CM.cpp common/CM/CM.h Peer/Peer.cpp Peer/Peer.h Peer/registry_stubs.cpp Peer/peer_stubs.cpp Peer/peer_svc.cpp)
add_executable(peer ${CLIENT_SOURCE_FILES})

set(REGISTRY_SOURCE_FILES common/Message/Message.h common/UDP/UDPSocket.h common/UDP/UDPSocket.cpp common/Message/Message.cpp common/registry_rpc.h common/peer_rpc.h common/CM/CM.cpp common/CM/CM.h Registry/Registry.cpp Registry/Registry.h Registry/registry_svc.cpp)
add_executable(registry ${REGISTRY_SOURCE_FILES})
