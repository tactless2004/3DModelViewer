#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    glm::vec3 cameraPos;
    glm::vec3 cameraFront;
    glm::vec3 cameraUp;
    float cameraTranslationSpeed;
    float cameraRotationSpeed;
    
    // Constructor
    Camera(glm::vec3 pos);


    void pollInput(GLFWwindow* window);
    glm::mat4 getView() const;
};