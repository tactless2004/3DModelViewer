#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "glad.h"
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Local imports
#include "ObjParser.hpp"
#include "gl_utils.hpp"
#include "model_renderer.hpp"
#include "grid_lines.hpp"

float angleX = 0.0f;
float angleY = 0.0f;
float zoom = 1.0f;

// Model position
glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraPos = glm::vec3(0, 0, 5);

// Define functions, even though modern c++ should be able to look ahead compile
void pollInput(GLFWwindow* window);

int main (int argc, char* argv[]) {
    ObjParser parser;
    parser.parseFile(argv[1]);
    parser.normalize();
    std::vector<GLfloat> vertbuf = parser.flatten();

    GLFWwindow* window;

    if (!glfwInit()) {
        return -1;
    }

    window = glfwCreateWindow(640, 480, "OBJ Viewer", NULL, NULL);
    glfwMakeContextCurrent(window);

    if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Couldn't find opengl" << std::endl;
        glfwTerminate();
        return -1;
    }

	// Compile model shaders
	GLuint programID = LoadShaders(
        "/home/leytonm/Dev/C/OBJViewerProject/shaders/vertshader.glsl",
        "/home/leytonm/Dev/C/OBJViewerProject/shaders/fragshader.glsl"
    );

	ModelRenderer model1 = ModelRenderer(programID, vertbuf, glm::vec3(0.0f, 0.0f, 0.0f));
    GridLines grid_lines = GridLines();

    // vertices never change, so just bufferData now
    std::cout << vertbuf.size() << std::endl;
    model1.bufferVertData();
    glEnable(GL_DEPTH_TEST);  

    glClearColor(1.0, 0.5, 0.0f, 1.0f);
    while(!glfwWindowShouldClose(window)) {
        glfwPollEvents();
		// poll events to move 3D objects
        pollInput(window);

        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		// actuate inputs
		model1.reset_model();
		model1.translate_model(position);
		model1.rotate_model(angleX, glm::vec3(1, 0, 0));
		model1.rotate_model(angleY, glm::vec3(0, 1, 0));
		model1.scale_model(zoom);
		
        // render model
        model1.render();

        // render gridlines
        grid_lines.render();
        
        // Swap the front and back buffers
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}

void pollInput(GLFWwindow* window) {
	// TODO: research how other tools do this, not happy with the current state of the movements
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) position.y += 0.05f;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) position.y -= 0.05f;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) position.x -= 0.05f;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) position.x += 0.05f;

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) angleX -= 2.0f;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) angleX += 2.0f;

    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) zoom *= 1.05f;
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) zoom /= 1.05f;
}
