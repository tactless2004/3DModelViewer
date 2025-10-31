cmake_minimum_required(VERSION 3.0.0)
project(obj_viewer VERSION 0.0.1)
cmake_policy(SET CMP0072 NEW)

find_package(OpenGL REQUIRED)

add_executable(obj_viewer
    src/main.cpp
    src/glad.h
    src/glad.c
    src/ObjParser.hpp
    src/ObjParser.cpp
    src/gl_utils.hpp
    src/gl_utils.cpp
)

target_link_libraries(obj_viewer
    glfw
    OpenGL::GL
)