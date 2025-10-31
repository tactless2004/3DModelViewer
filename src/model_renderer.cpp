#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "model_renderer.hpp"

ModelRenderer::ModelRenderer(GLuint programID, std::vector<GLfloat>& verts, glm::vec3 position) :
    renderingProgram(programID), vertices(verts), pos(position) {
    // Initialize matrices
    model = glm::mat4(1.0f);
    model = glm::translate(model, position);

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
        glm::vec3(model[0].x, model[1].y, model[2].z)
    );

    glm::mat4 inv_init_pos = glm::translate(
        glm::mat4(1.0f),
        glm::vec3(-model[0].x, -model[1].y, -model[2].z)
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