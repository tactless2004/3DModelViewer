#include "gl_utils.hpp"
#include "grid_lines.hpp"
#include "glad.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

GridLines::GridLines() {
    renderingProgram = LoadShaders(
        "../shaders/grid_lines.vs",
        "../shaders/grid_lines.fs"
    );

    
    glGenVertexArrays(
        1,   // length
        &VAO // location of vao array (in this case one vao so no arr)
    );

    glBindVertexArray(VAO);
    glGenBuffers(1, &vertexVBO);
    createGridlines();

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    // Explicitly unbind vao
    glBindVertexArray(0);


    projection = glm::perspective(
		glm::radians(45.0f), 
		640.0f / 480.0f, // aspect ratio
		0.1f,
		100.0f
	);
}

void GridLines::createGridlines() {
    float MINW = -10.0f;
    float MINH = -10.0f;
    float MAXW = 10.0f;
    float MAXH = 10.0f;
    float interval = 0.1f;

    // horizontal lines
    float currentHeight = MINH;
    while(currentHeight <= MAXH) {
        vertices.push_back(glm::vec3(MINW, 0.0f, currentHeight));
        vertices.push_back(glm::vec3(MAXW, 0.0f, currentHeight));

        currentHeight += interval;
    }

    // vertical lines
    float currentWidth = MINW;
    while(currentWidth <= MAXW) {
        vertices.push_back(glm::vec3(currentWidth, 0.0f, MINH));
        vertices.push_back(glm::vec3(currentWidth, 0.0f, MAXH));

        currentWidth += interval;
    }

    
    
    glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
    glBufferData(
        GL_ARRAY_BUFFER,
        vertices.size() * sizeof(glm::vec3),
        vertices.data(),
        GL_STATIC_DRAW
    );
}

void GridLines::setModel(glm::mat4 m) {model = m;}
void GridLines::setView(glm::mat4 m) {view = m;}
void GridLines::setProjection(glm::mat4 m) {projection = m;} 

void GridLines::render() const {
    glUseProgram(renderingProgram);
    GLint mvpLoc = glGetUniformLocation(renderingProgram, "mvp");

    glUniformMatrix4fv(
        mvpLoc,
        1,
        GL_FALSE,
        glm::value_ptr(projection * view * model)
    );

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);

    glVertexAttribPointer(
        0,        // layout = 0 vertex in
        3,        // three values per shader
        GL_FLOAT, // data type
        GL_FALSE, // normalized ?
        0,        // stride (offset between consecutive values) 0 means they are "tightly packed"
        (void*) 0 // ptr to the first value, skip none, so 0
    );
    glDrawArrays(GL_LINE_LOOP, 0, vertices.size());
}