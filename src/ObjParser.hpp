#pragma once
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include <array>

struct Vertex {
    glm::vec3 pos;
};

struct Face {
    int v1, v2, v3;
};

class ObjParser {
public:
    std::vector<Vertex> vertdat;
    std::vector<Face> facedat;

    void parseFile(std::string f);
    void normalize();
    std::vector<GLfloat> flatten() const;
};