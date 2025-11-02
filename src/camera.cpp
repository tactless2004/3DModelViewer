#include "camera.hpp"

Camera::Camera(glm::vec3 pos) {
    cameraPos = pos;
    cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);

    cameraTranslationSpeed = 0.05f;
}

void Camera::pollInput(GLFWwindow* window) {

    // FWD
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        cameraPos += cameraTranslationSpeed * cameraFront;
    } 

    // BACK
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        cameraPos -= cameraTranslationSpeed * cameraFront;
    }

    // LEFT
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraTranslationSpeed;
    }

    // RIGHT
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraTranslationSpeed;
    }
}

glm::mat4 Camera::getView() const {
    return glm::lookAt(
        cameraPos,
        cameraFront + cameraPos,
        cameraUp
    );
}