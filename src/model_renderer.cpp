#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream> // temprorary debug print
#include "model_renderer.hpp"

ModelRenderer::ModelRenderer(GLuint programID, std::vector<GLfloat> verts, glm::vec3 position) :
    renderingProgram(programID), vertices(verts), pos(position) {
    // Initialize matrices
    model = glm::mat4(1.0f);
    model = glm::translate(model, position);

    // Generate and bind VBO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);	

    // Generate vbo
    glGenBuffers(1, &vertexVBO);

    // TODO: handle view matrix in a Camera Class
	view = glm::lookAt(
		glm::vec3(0, 0, 5), // camera vec
		glm::vec3(0, 0, 0), // center vec
		glm::vec3(0, 1, 0) // up vec
	);

    projection = glm::perspective(
		glm::radians(45.0f), 
		640.0f / 480.0f, // aspect ratio
		0.1f,
		100.0f
	);
}

void ModelRenderer::translate_model(GLfloat x, GLfloat y, GLfloat z) {
    model = glm::translate(model, glm::vec3(x, y, z));
}

void ModelRenderer::translate_model(glm::vec3 transvec) {
    model = glm::translate(model, transvec);
}

void ModelRenderer::rotate_model(GLfloat radians, glm::vec3 axis) {
    // rotate with respect to the origin
    glm::mat4 init_pos = glm::translate(
        glm::mat4(1.0f),
        glm::vec3(model[3])
    );

    glm::mat4 inv_init_pos = glm::translate(
        glm::mat4(1.0f),
        glm::vec3(-model[3])
    );

    // Translate model to the origin
    model = inv_init_pos * model;

    // Apply the rotation
    model = glm::rotate(model, glm::radians(radians), axis);

    // Translate model to its position pre rotation
    model = init_pos * model;
    
}

void ModelRenderer::scale_model(GLfloat sf) {
    model = glm::scale(model, glm::vec3(sf));
}

void ModelRenderer::reset_model() {
    model = glm::mat4(1.0f);
}
glm::mat4 ModelRenderer::get_mvp() const {
    glm::mat4 mvp;
    mvp = projection * view * model;
    return mvp;
}

void ModelRenderer::bufferVertData() {
    glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
    glBufferData(
        GL_ARRAY_BUFFER,
        vertices.size() * sizeof(GLfloat),
        vertices.data(),
        GL_STATIC_DRAW
    );
}

void ModelRenderer::render() const {
    // Get MVP location
	GLint mvpLoc = glGetUniformLocation(renderingProgram, "mvp");

    glUseProgram(renderingProgram);

    // Give OpenGL the MVP uniform
    glUniformMatrix4fv(
        mvpLoc,
        1,
        GL_FALSE,
        glm::value_ptr(get_mvp())
    );

    
    // Draw the OBJ
    glBindVertexArray(VAO);
    // std::cout << "VAO: " << VAO << std::endl;
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
    glVertexAttribPointer(
        0,        // layout = 0 vertex in
        3,        // three values per shader
        GL_FLOAT, // data type
        GL_FALSE, // normalized ?
        0,        // stride (offset between consecutive values) 0 means they are "tightly packed"
        (void*) 0 // ptr to the first value, skip none, so 0
    );
    glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 3);
    glDisableVertexAttribArray(0);

}