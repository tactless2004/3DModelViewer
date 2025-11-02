#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
private:
    // lookAt (view) matrix components
    glm::vec3 cameraPos;
    glm::vec3 cameraFront;
    glm::vec3 cameraUp;

    // Camera Movement speeds
    float cameraTranslationSpeed;
    float cameraRotationSpeed;

    // Frame timing stuff
    double prevTime;
    double deltaTime;
    
public:
    // Constructor
    Camera(glm::vec3 pos);

    void pollInput(GLFWwindow* window);
    glm::mat4 getView() const;
};