cmake_minimum_required(VERSION 3.26)
project(MazeGenerator)

set(CMAKE_CXX_STANDARD 17)

add_executable(MazeGenerator main.cpp olcPixelGameEngine.h)
target_link_libraries(MazeGenerator pulse pulse-simple X11 GL pthread png "stdc++fs")
