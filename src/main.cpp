#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "btBulletDynamicsCommon.h"
#include "btBulletCollisionCommon.h"

#include "shader.hpp"
#include "camera.hpp"
#include "model.hpp"
#include "terrain.hpp"
#include "player.hpp"
#include "rigidbodyentity.hpp"
#include "UIElement.hpp"
#include "skybox.hpp"
#include "uitextelement.hpp"
#include "dialogue.hpp"
#include "billboardentity.hpp"
#include "piggy.hpp"
#include "door.hpp"
#include "trashbag.hpp"
#include "uimaster.hpp"
#include "progressbar.hpp"
#include "mainscene.hpp"

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow *window, double xposIn, double yposIn);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);


bool firstMouse = true;
// camera
Camera camera(glm::vec3(0.0f, 10.0f, 3.0f));
// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "ggg", NULL, NULL);

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    UIMaster ui(SCR_WIDTH, SCR_HEIGHT);
    UITextElement crosshair("resources/text/Angelic Peace.ttf", "X", 48);
    ui.addTextElement(crosshair);

    float progress = 0.0f;
    MainScene scene1("scene1", ui, camera);
    ProgressBar progressBar1;
    crosshair.setText("Please be patient with me...");
    scene1.initialize([&progress, &window, &progressBar1, &crosshair](float newProgress) { // i tyhinks this workds lol
        float curTime = static_cast<float>(glfwGetTime());
        glClearColor(0.05f, 0.05f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        progress += newProgress;
        progressBar1.update(progress);
        crosshair.render((float)SCR_WIDTH / 4, (float)SCR_HEIGHT / 2, 1.0, glm::vec3(1.0, 0.0, 0.0), curTime);
        glfwSwapBuffers(window);
        glfwPollEvents();
    });

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        scene1.render(deltaTime, currentFrame, window);
    }

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}