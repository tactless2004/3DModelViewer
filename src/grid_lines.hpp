#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class GridLines {
public:
    // constructor
    GridLines();

    // public methods
    void setProjection(glm::mat4 m);
    void setView(glm::mat4 m);
    void setModel(glm::mat4 m);

    void render() const;
private:
    GLuint renderingProgram, VAO, vertexVBO;
    glm::mat4 projection, view, model;

    std::vector<glm::vec3> vertices;

    // private methods
    void createGridlines();
};