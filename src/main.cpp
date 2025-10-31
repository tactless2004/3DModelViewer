#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glad.h"
#include "ObjParser.hpp"
#include <GLFW/glfw3.h>

float angleX = 0.0f;
float angleY = 0.0f;
float zoom = 1.0f;
glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);

GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path);
void pollInput(GLFWwindow* window);

int main () {

    ObjParser parser;
    parser.parseFile("/home/leytonm/Dev/C/OBJViewerProject/test_obj_files/teapot.obj");
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

    glClearColor(0.25f, 0.5f, 0.75f, 1.0f);
    GLuint programID = LoadShaders(
        "/home/leytonm/Dev/C/OBJViewerProject/shaders/vertshader.glsl",
        "/home/leytonm/Dev/C/OBJViewerProject/shaders/fragshader.glsl"
    );
    glUseProgram(programID);
    GLuint vao, vbo;
    // Generate vao
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Generate vbo
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // Load data into vbo
    glBufferData(
        GL_ARRAY_BUFFER,
        vertbuf.size() * sizeof(GLfloat),
        vertbuf.data(),
        GL_STATIC_DRAW
    );
    
	// Get MVP location
	GLint mvpLoc = glGetUniformLocation(programID, "mvp");

    while(!glfwWindowShouldClose(window)) {
        glfwPollEvents();
		// poll events to move 3D objects
        pollInput(window);

		// Change model matrix based on user inputs
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, position);
		model = glm::rotate(model, glm::radians(angleX), glm::vec3(1, 0, 0));
		model = glm::rotate(model, glm::radians(angleY), glm::vec3(0, 1, 0));
		model = glm::scale(model, glm::vec3(zoom));

		// generate view matrix
		glm::mat4 view = glm::lookAt(
			glm::vec3(0, 0, 5), // camera vec
			glm::vec3(0, 0, 0), // center vec
			glm::vec3(0, 1, 0) // up vec
		);

		glm::mat4 projection = glm::perspective(
			glm::radians(45.0f), 
			640.0f / 480.0f, // aspect ratio
			0.1f,
			100.0f
		);

		glm::mat4 mvp = projection * view * model;

		glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Draw the OBJ file :)
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glVertexAttribPointer(
            0,
            3,
            GL_FLOAT,
            GL_FALSE,
            0,
            (void*)0
        );
        glDrawArrays(GL_TRIANGLES, 0, vertbuf.size() / 3);
        glDisableVertexAttribArray(0);

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}

void pollInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) position.y -= 0.05f;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) position.y += 0.05f;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) angleX += 2.0f;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) angleX -= 2.0f;

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) angleY -= 2.0f;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) angleY += 2.0f;

    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) zoom *= 1.05f;
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) zoom /= 1.05f;
}
// LoadShaders code from:
// https://www.opengl-tutorial.org/beginners-tutorials/tutorial-2-the-first-triangle/
GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path){

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if(VertexShaderStream.is_open()){
		std::stringstream sstr;
		sstr << VertexShaderStream.rdbuf();
		VertexShaderCode = sstr.str();
		VertexShaderStream.close();
	}else{
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
		getchar();
		return 0;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if(FragmentShaderStream.is_open()){
		std::stringstream sstr;
		sstr << FragmentShaderStream.rdbuf();
		FragmentShaderCode = sstr.str();
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}

	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}

	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> ProgramErrorMessage(InfoLogLength+1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}
	
	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);
	
	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}