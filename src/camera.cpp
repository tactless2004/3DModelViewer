#include "camera.hpp"

Camera::Camera(glm::vec3 pos) {
    cameraPos = pos;
    cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);

    cameraTranslationSpeed = 0.05f;
    prevTime = glfwGetTime();
}

void Camera::pollInput(GLFWwindow* window) {
    // Frame timing stuff
    deltaTime = glfwGetTime() - prevTime;
    float thisFrameTranslationSpeed = (float) cameraTranslationSpeed * deltaTime;

    // FWD
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        cameraPos += thisFrameTranslationSpeed * cameraFront;
    } 

    // BACK
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        cameraPos -= thisFrameTranslationSpeed * cameraFront;
    }

    // LEFT
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * thisFrameTranslationSpeed;
    }

    // RIGHT
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * thisFrameTranslationSpeed;
    }
}

glm::mat4 Camera::getView() const {
    return glm::lookAt(
        cameraPos,
        cameraFront + cameraPos,
        cameraUp
    );
}