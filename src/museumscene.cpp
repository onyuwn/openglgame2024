#include "museumscene.hpp"

MuseumScene::MuseumScene(std::string name, UIMaster &ui, Camera &camera) : initialized(false),
                             ui(ui), camera(camera), physDebugOn(false) {

}

void MuseumScene::render(float deltaTime, float curTime, GLFWwindow *window) {
    if(this->initialized) {
        this->world->stepSimulation(deltaTime, 7);
        this->player->UpdatePlayer(curTime, deltaTime, window);

        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom * 2.0f), (float)800 / (float)600, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix(player->getPlayerPos() + glm::vec3(0,1,0));

        this->basicShader->use();
        basicShader->setMat4("projection", projection);
        basicShader->setMat4("view", view);

        glm::mat4 model = glm::mat4(1.0f);
        basicShader->setMat4("model", model);
        this->museumItemsModel->draw(*this->basicShader);
        this->player->render(curTime, deltaTime);

        this->terrain->render(*this->basicShader);
        this->skybox->render(glm::mat4(glm::mat3(view)), projection);

        this->ui.render(deltaTime, curTime);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void MuseumScene::initialize(std::function<void(float, std::string)> progressCallback) {
    this->basicShader = std::make_shared<Shader>("src/shaders/basic.vs", "src/shaders/basic.fs");
    std::vector<std::string> skyboxFaces = { // rgba
        "resources/mainskybox/gradiesn.png",
        "resources/mainskybox/gradiesn.png",
        "resources/mainskybox/py.png",
        "resources/mainskybox/ny.png",
        "resources/mainskybox/gradiesn.png",
        "resources/mainskybox/gradiesn.png",
    };
    this->skybox = std::make_shared<Skybox>(skyboxFaces);

    btBroadphaseInterface* broadphase = new btDbvtBroadphase();
    btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
    btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
    btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;
    this->world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);

    this->museumItemsModel = std::make_shared<Model>((char*)"resources/buildings/museum/museumitems1.obj");
    this->museumModel = std::make_shared<Model>((char*)"resources/buildings/museum/museum1.obj");
    progressCallback(.05f, "loading museum...");
    this->terrain = std::make_shared<Terrain>(*museumModel);
    terrain->initTerrain();
    terrain->addToWorld(world);

    this->player = std::make_shared<Player>(camera, this->world, ui, physDebugOn, "resources/characters/arms4.gltf");
    player->initialize();
    this->player->addToWorld(this->world);
    progressCallback(.1f, "initializing player...");
    this->initialized = true;
}

void MuseumScene::addGameObject(std::shared_ptr<GameObject> gameObject) {

}

void MuseumScene::removeGameObject() {

}