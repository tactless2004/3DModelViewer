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
                vertdat.push_back({x, y, z});
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
        vertex.x * vertex.x +
        vertex.y * vertex.y +
        vertex.z * vertex.z
    );
}

void ObjParser::normalize() {
    float max_dim = 0.0f;
    for (auto& vertex : vertdat) {
        max_dim = std::max({max_dim, vertex.x, vertex.y, vertex.z});
    }

    float inv_max_dim = 1.0f / max_dim ? max_dim != 0 : 1.0f;

    for (auto& vertex : vertdat) {
        vertex.x *= inv_max_dim;
        vertex.y *= inv_max_dim;
        vertex.z *= inv_max_dim;
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
                v1.x, v1.y, v1.z,
                v2.x, v2.y, v2.z,
                v3.x, v3.y, v3.z
            }
        );
    }

    return flat;
}