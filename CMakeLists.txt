cmake_minimum_required(VERSION 3.16)
project(finder)

set(CMAKE_CXX_STANDARD 17)
include_directories(INCLUDE_DIRS project/include)
set(SRC project/src/main.cpp project/src/Application.cpp project/src/Database.cpp project/src/FileServer.cpp project/src/RequestParser.cpp project/src/TcpClient.cpp)

add_executable(finder ${SRC})