cmake_minimum_required(VERSION 3.10)
project(3DMinesweeper)

set(CMAKE_CXX_STANDARD 11)

link_libraries(GL)
link_libraries(GLU)
link_libraries(glut)

add_executable(main main.c)
