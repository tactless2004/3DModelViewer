#include "gl_utils.hpp"
#include "grid_lines.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

GridLines::GridLines() {
    renderingProgram = LoadShaders(
        "../shaders/grid_lines.vs",
        "../shaders/grid_lines.fs"
    );
}

void GridLines::render() const {
    GLint gVPLoc = glGetUniformLocation(renderingProgram, "gVP");
    glUniformMatrix4fv(
        gVPLoc,
        1,
        GL_FALSE,
        glm::value_ptr(
            // hard code for now
            glm::lookAt(
        		glm::vec3(0, 0, 5), // camera vec
        		glm::vec3(0, 0, 0), // center vec
        		glm::vec3(0, 1, 0) // up vec
        	)
        )
    );
    glUseProgram(renderingProgram);
    glDrawArrays(GL_TRIANGLES, 0, 4);
}