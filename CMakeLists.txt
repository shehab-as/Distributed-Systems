cmake_minimum_required(VERSION 3.6)
project(Distributed-Systems)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")
include_directories(SQLiteCpp/include)
add_subdirectory(SQLiteCpp)
set(CLIENT_SOURCE_FILES common/Message/Message.h common/UDP/UDPSocket.h common/UDP/UDPSocket.cpp common/Message/Message.cpp common/CM/CM.cpp common/CM/CM.h Peer/Peer.cpp Peer/Peer.h common/Base64/base64.cpp common/Base64/base64.h)
add_executable(peer ${CLIENT_SOURCE_FILES})

set(REGISTRY_SOURCE_FILES common/Message/Message.h common/UDP/UDPSocket.h common/UDP/UDPSocket.cpp common/Message/Message.cpp common/CM/CM.cpp common/CM/CM.h Registry/Registry.cpp Registry/Registry.h Registry/main.cpp common/Base64/base64.cpp common/Base64/base64.h)
add_executable(registry ${REGISTRY_SOURCE_FILES})
target_link_libraries(registry SQLiteCpp sqlite3 dl)
target_link_libraries(peer SQLiteCpp sqlite3 dl)

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Gui REQUIRED)

set(PEER_QT_SOURCE_FILES Image-Viewer/main.cpp Image-Viewer/mainwindow.cpp Image-Viewer/mainwindow.h common/Message/Message.h common/UDP/UDPSocket.h common/UDP/UDPSocket.cpp common/Message/Message.cpp common/CM/CM.cpp common/CM/CM.h Peer/Peer.cpp Peer/Peer.h common/Base64/base64.cpp common/Base64/base64.h)
qt5_wrap_ui(PEER_QT_UI Image-Viewer/mainwindow.ui)
add_executable(peer_qt ${PEER_QT_SOURCE_FILES} ${PEER_QT_UI})
qt5_use_modules(peer_qt Core Widgets Gui)
target_link_libraries(peer_qt SQLiteCpp sqlite3 dl)
