cmake_minimum_required(VERSION 3.10)
project(3DMinesweeper)

set(CMAKE_CXX_STANDARD 11)

link_libraries(GL)
link_libraries(GLU)
link_libraries(glut)
link_libraries(m)

add_executable(main main.cpp Field.cpp Field.hpp Board.cpp Board.hpp Grid.cpp Grid.hpp Mine.cpp Mine.hpp BMPLoader.cpp BMPLoader.hpp)