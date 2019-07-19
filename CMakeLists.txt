cmake_minimum_required(VERSION 3.14)
project(Sudoku)

set(CMAKE_CXX_STANDARD 17)

add_executable(Sudoku main.cpp Sudoku9.cpp Rand.cpp)