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

    // std::vector<std::string> skyboxFaces = { // rgba
    //     "resources/mainskybox/gradiesn.png",
    //     "resources/mainskybox/gradiesn.png",
    //     "resources/mainskybox/py.png",
    //     "resources/mainskybox/ny.png",
    //     "resources/mainskybox/gradiesn.png",
    //     "resources/mainskybox/gradiesn.png",
    // };
    // Skybox mainSkybox(skyboxFaces);
    // stbi_set_flip_vertically_on_load(true);

    UIMaster ui(SCR_WIDTH, SCR_HEIGHT);
    //Shader basicShader("src/shaders/basic.vs", "src/shaders/basic.fs");
    // Model gModel("resources/g.obj");
    // stbi_set_flip_vertically_on_load(false);
    // Model arrowsModel("resources/g2.obj");
    // Model deskModel("resources/desk/desk.obj");
    // Model kitchenModel("resources/buildings/kitchen/kitcheninterior2.obj");
    // Model kitchenItems("resources/buildings/kitchen/kitchenItems1.obj");
    // Model kitchenDoors("resources/buildings/kitchen/kitchendoors1.obj");
    // Model piggyModel("resources/piggyiso.obj");
    // Model trashBagModel("resources/trash/trashbag.obj");

    UITextElement crosshair("resources/text/Angelic Peace.ttf", "X", 48);
    ui.addTextElement(crosshair);
    // // models need to load separately from game objects
    // Piggy piggy("piggy1", basicShader, piggyModel);
    // piggy.initialize();

    // TrashBag trashBag1(basicShader, trashBagModel);
    // trashBag1.initialize();

    // Door door1(basicShader, kitchenDoors, glm::vec3(8.7, 1.86, 10.42)); // just get these from blender
    // door1.initialize();

    // stbi_set_flip_vertically_on_load(true);
    // BillboardEntity gBillboard(arrowsModel, glm::vec3(0, 15, 0), glm::vec2(1.0, 1.0));
    
    // Terrain kitchen(kitchenModel);
    // kitchen.initTerrain();

    // btDiscreteDynamicsWorld* dynamicsWorld;
    // btBroadphaseInterface* broadphase = new btDbvtBroadphase();
    // btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
    // btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
    // btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;
    // dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);

    // kitchen.addToWorld(dynamicsWorld);
    // player.addToWorld(dynamicsWorld);
    // piggy.addToWorld(dynamicsWorld);
    // trashBag1.addToWorld(dynamicsWorld);
    // door1.addToWorld(dynamicsWorld);
    // dynamicsWorld->setGravity(btVector3(0,-9.81f,0));

    float progress = 0.0f;
    MainScene scene1("scene1", ui, camera);
    ProgressBar progressBar1;
    scene1.initialize([&progress, &window, &progressBar1](float newProgress) { // i tyhinks this workds lol
        glClearColor(0.05f, 0.05f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        progress += newProgress;
        progressBar1.update(progress);
        glfwSwapBuffers(window);
        glfwPollEvents();
    });

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        scene1.render(deltaTime, currentFrame, window);
		// dynamicsWorld->stepSimulation(deltaTime, 7);

        // player.UpdatePlayer(deltaTime, window);

        // glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // // don't forget to enable shader before setting uniforms
        // basicShader.use();

        // // view/projection transformations
        // glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        // glm::mat4 view = camera.GetViewMatrix(player.getPlayerPos());
        // //view = glm::translate(view, player.getPlayerPos());
        // basicShader.setMat4("projection", projection);
        // basicShader.setMat4("view", view);

        // // render the loaded model
        // door1.render(deltaTime);
        // kitchen.render(basicShader);
        // kitchenItems.draw(basicShader);

        // glm::mat4 model = glm::mat4(1.0f);
        // model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        // basicShader.setMat4("model", model);
        // trashBag1.render(deltaTime);

        // model = glm::translate(model, glm::vec3(10.0f, -0.5f, 2.0f));
        // model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
        // basicShader.setMat4("model", model);
        // deskModel.draw(basicShader);

        // model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
        // model = glm::translate(model, glm::vec3(10.0f, -0.5f, 2.0f));

        // piggy.render(deltaTime);
        // gBillboard.render(view, projection, camera.Up, camera.Right); // follows pig
        // glm::vec3 pigPos = piggy.getPos();
        // gBillboard.setPos(glm::vec3(pigPos.x, pigPos.y + 1.25f, pigPos.z));

        // mainSkybox.render(glm::mat4(glm::mat3(view)), projection);

        // ui.render(deltaTime, currentFrame);

        // glfwSwapBuffers(window);
        // glfwPollEvents();
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