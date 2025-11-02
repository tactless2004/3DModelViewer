#include <GLFW/glfw3.h>

class GridLines {
public:
    GLuint renderingProgram;

    // constructor
    GridLines();

    // local methods
    void render() const;
};