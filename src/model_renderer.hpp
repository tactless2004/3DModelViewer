#include <vector>
#include <glm/glm.hpp>
#include "glad.h"

class ModelRenderer {
public:
    // OpenGL internal stuff
    GLuint renderingProgram;
    GLuint vertexVBO;
    GLuint VAO;
    
    // Matrices
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;

    // Vectors
    glm::vec3 pos;

    // Collections
    std::vector<GLfloat> vertices;

    // Constructor
    ModelRenderer(const GLuint programID, const std::vector<GLfloat> verts, const glm::vec3 position);

    // Transform Methods
    void reset_model();
    void translate_model(const GLfloat x, const GLfloat y, const GLfloat z);
    void translate_model(const glm::vec3 transvec);

    void rotate_model(const GLfloat radians, const glm::vec3 axis);

    void scale_model(const GLfloat sf);

    // Render Methods
    void bufferVertData();
    void render() const;

    glm::mat4 get_mvp() const;
};