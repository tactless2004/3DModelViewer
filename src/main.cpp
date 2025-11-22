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
#include "camera.hpp"

float angleX = 0.0f;
float angleY = 0.0f;
float zoom = 1.0f;

// Model position
glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraPos = glm::vec3(0, 0, 5);

void glfw_windowsize_change_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);

    glm::mat4 newProjectionMat = glm::perspective(
        glm::radians(45.0f),
        static_cast<float>(width) / height,
        0.1f,
        100.0f
    );

    ModelRenderer* renderer = static_cast<ModelRenderer*>(glfwGetWindowUserPointer(window));
    GridLines* grid_lines = static_cast<GridLines*>(glfwGetWindowUserPointer(window));

    renderer->setProjection(newProjectionMat);
    grid_lines->setProjection(newProjectionMat);
}

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
    // Attach resize callback
    glfwSetFramebufferSizeCallback(window, glfw_windowsize_change_callback);

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

    Camera camera = Camera(glm::vec3(0.0f, 0.0f, 5.0f));
	ModelRenderer model1 = ModelRenderer(programID, vertbuf, glm::vec3(0.0f, 0.0f, 0.0f));

    GridLines grid_lines = GridLines();
    // Add window pointer, so glfw callbacks can see the model and gridlines.
    // this is just to update the projection matrix.
    glfwSetWindowUserPointer(window, &model1);
    glfwSetWindowUserPointer(window, &grid_lines);

    // vertices never change, so just bufferData now
    model1.bufferVertData();
    glEnable(GL_DEPTH_TEST);  

    // Cornflower Blue
    glClearColor(0.392, 0.584, 0.929f, 1.0f);

    while(!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        camera.pollInput(window);
        model1.setView(camera.getView());

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
        grid_lines.setModel(glm::mat4(1.0f));
        grid_lines.setView(camera.getView());
        grid_lines.render();
        
        // Swap the front and back buffers
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}