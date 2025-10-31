#pragma once
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <array>

struct Vertex {
    float x,y,z;
};

struct Face {
    int v1, v2, v3;
};

class ObjParser {
public:
    std::vector<Vertex> vertdat;
    std::vector<Face> facedat;

    void parseFile(std::string f);
    std::vector<GLfloat> flatten() const;
};