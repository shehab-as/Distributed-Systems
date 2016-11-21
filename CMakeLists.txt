cmake_minimum_required(VERSION 3.6)
project(Distributed-Systems)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")
include_directories(SQLiteCpp/include)
add_subdirectory(SQLiteCpp)
set(CLIENT_SOURCE_FILES  common/Message/Message.h common/UDP/UDPSocket.h common/UDP/UDPSocket.cpp common/Message/Message.cpp common/CM/CM.cpp common/CM/CM.h Peer/Peer.cpp Peer/Peer.h)
add_executable(peer ${CLIENT_SOURCE_FILES})

set(REGISTRY_SOURCE_FILES common/Message/Message.h common/UDP/UDPSocket.h common/UDP/UDPSocket.cpp common/Message/Message.cpp common/CM/CM.cpp common/CM/CM.h Registry/Registry.cpp Registry/Registry.h DataBase/Db.h DataBase/Db.cpp Registry/main.cpp)
add_executable(registry ${REGISTRY_SOURCE_FILES})
target_link_libraries(registry SQLiteCpp sqlite3 dl)
target_link_libraries(peer SQLiteCpp sqlite3 dl)
