#include "mainscene.hpp"

MainScene::MainScene(std::string name) : initialized(false), skybox(nullptr), world(nullptr), terrain(nullptr) {}

void MainScene::render(float deltaTime, float curTime) {
    if(this->initialized) {

    }
}

void MainScene::initialize(std::function<void(float)> progressCallback) {
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
    progressCallback(.1f);
    btBroadphaseInterface* broadphase = new btDbvtBroadphase();
    btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
    btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
    btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;
    this->world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
    progressCallback(.1f);
    stbi_set_flip_vertically_on_load(false);
    this->piggyModel = std::make_shared<Model>((char*)"piggy_model.obj");
    std::shared_ptr<Piggy> piggy = std::make_shared<Piggy>("Piggy", *basicShader, *piggyModel);
    piggy->initialize();
    progressCallback(.1f);

    this->kitchenModel = std::make_shared<Model>((char*)"resources/buildings/kitchen/kitcheninterior2.obj");
    piggy->addToWorld(this->world);
    progressCallback(.1f);
    this->initialized = true;
    progressCallback(.6f);
}

void MainScene::addGameObject(std::shared_ptr<GameObject> gameObject) {

}

void MainScene::removeGameObject() {

}