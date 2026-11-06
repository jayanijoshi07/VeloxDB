
cmake_minimum_required(VERSION 3.16)

project(VeloxDB LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(server
    server.cpp
    src/protocol.cpp
)

add_executable(client
    client.cpp
    src/protocol.cpp
)

target_include_directories(server PRIVATE include)
target_include_directories(client PRIVATE include)
