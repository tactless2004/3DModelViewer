#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <glm/glm.hpp>
#include "ObjParser.hpp"


void ObjParser::parseFile(std::string f) {
    std::ifstream inputFile(f);

    // Check if there was an error in opening the file
    if(!inputFile.is_open()) {
        std::cerr << "Error opening file " << f << "!" << std::endl;
        return;
    }

    std::string line;

    // read each line
    while(std::getline(inputFile, line)) {
        std::stringstream vertstream(line);
        std::string prefix;
        float x, y, z;
        int v1, v2, v3;

        vertstream >> prefix;

        // TODO: make this a switch statement
        if(prefix == "v") {
            if(vertstream >> x >> y >> z) {
                vertdat.push_back(
                    { //begin Vertex Struct
                        glm::vec3(x, y, z) // Vertex.pos (glm::vec3)
                    } //end Vertex Struct
                );
            }
        } else if(prefix == "f") {
            if(vertstream >> v1 >> v2 >> v3) {
                facedat.push_back({v1 - 1, v2 - 1, v3 - 1});
            }
        } else {
            std::cout << "Unknown line type " << prefix << std::endl;
        }
    }

    if (inputFile.fail()) {
        std::cerr << "An error occurred during readin the .obj file." << std::endl;
    }

    inputFile.close();
}

float v_mag_f(const Vertex& vertex) {
    return (float) glm::sqrt(
        vertex.pos.x * vertex.pos.x +
        vertex.pos.y * vertex.pos.y +
        vertex.pos.z * vertex.pos.z
    );
}

void ObjParser::normalize() {
    float max_dim = 0.0f;

    // First pass find the maximum dimension size
    for (auto& vertex : vertdat) {
        max_dim = std::max({max_dim, vertex.pos.x, vertex.pos.y, vertex.pos.z});
    }

    float inv_max_dim = 1.0f / max_dim ? max_dim != 0 : 1.0f;

    // Second pass inversely scale all dimenisons such that maximum dimension size = 1
    for (auto& vertex : vertdat) {
        vertex.pos.x *= inv_max_dim;
        vertex.pos.y *= inv_max_dim;
        vertex.pos.z *= inv_max_dim;
    }
}

std::vector<GLfloat> ObjParser::flatten() const {
    std::vector<GLfloat> flat;

    flat.reserve(facedat.size() * 9); // 9 floats (3 vertices) per face.

    for (const auto& face : facedat) {
        const Vertex& v1 = vertdat[face.v1];
        const Vertex& v2 = vertdat[face.v2];
        const Vertex& v3 = vertdat[face.v3];

        flat.insert(
            flat.end(),
            {
                v1.pos.x, v1.pos.y, v1.pos.z,
                v2.pos.x, v2.pos.y, v2.pos.z,
                v3.pos.x, v3.pos.y, v3.pos.z
            }
        );
    }

    return flat;
}