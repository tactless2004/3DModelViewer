#include <iostream>
#include <fstream>
#include <sstream>
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
                vertdat.push_back({x * 0.3f, y * 0.3f, z * 0.3f});
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

float v_mag_f(Vertex& vertex) {
    return (float) glm::sqrt(
        glm::pow(vertex.x, 2) +
        glm::pow(vertex.y, 2) +
        glm::pow(vertex.z, 2)
    );
}

void ObjParser::normalize() {
    for (auto& vertex : vertdat) {
        float mag = v_mag_f(vertex);
        vertex.x *= (1/mag);
        vertex.y *= (1/mag);
        vertex.z *= (1/mag);
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