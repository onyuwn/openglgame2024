#include "mainscene.hpp"

MainScene::MainScene(std::string name, UIMaster &ui, Camera &camera) : initialized(false),
                                                                       ui(ui), camera(camera), physDebugOn(false) {}

void MainScene::render(float deltaTime, float curTime, GLFWwindow *window) {
    if(this->initialized) {
        this->world->stepSimulation(deltaTime, 7);
        this->player->UpdatePlayer(deltaTime, window);

        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)800 / (float)600, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix(player->getPlayerPos());
        //glm::mat4 view = camera.GetViewMatrix(glm::vec3(0,0,0));

        this->basicShader->use();
        basicShader->setMat4("projection", projection);
        basicShader->setMat4("view", view);

        for(int i = 0; i < this->gameObjects.size(); i++) {
            this->gameObjects[i]->render(deltaTime);
        }
        glm::mat4 model = glm::mat4(1.0f);
        basicShader->setMat4("model", model);
        this->kitchenItemsModel->draw(*this->basicShader);

        for(int i = 0; i < this->models.size(); i++) {
            model = glm::translate(model, glm::vec3(1.0, 2.0, 1.0));
            model = glm::scale(model, glm::vec3(.25, .25, .25));
            basicShader->setMat4("model", model);
            this->models[i]->draw(*this->basicShader, curTime);
        }

        this->terrain->render(*this->basicShader);
        this->skybox->render(glm::mat4(glm::mat3(view)), projection);

        if(this->physDebugOn) {
            debugDrawer->SetMatrices(view, projection);
            world->debugDrawWorld();
        }
        this->ui.render(deltaTime, curTime);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void MainScene::initialize(std::function<void(float, std::string)> progressCallback) {
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
    progressCallback(.1f, "creating skybox");
    btBroadphaseInterface* broadphase = new btDbvtBroadphase();
    btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
    btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
    btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;
    this->world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);

    this->debugDrawer = std::make_shared<DebugDrawer>();
    debugDrawer->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
    world->setDebugDrawer(debugDrawer.get());

    progressCallback(.1f, "initializing physics...");
    stbi_set_flip_vertically_on_load(false);
    world->setGravity(btVector3(0,-9.81f,0));

    this->piggyModel = std::make_shared<Model>((char*)"resources/piggyiso.obj");
    std::shared_ptr<Piggy> piggy = std::make_shared<Piggy>("Piggy", *basicShader, *piggyModel);
    piggy->initialize();
    this->addGameObject(piggy);
    piggy->addToWorld(this->world);
    progressCallback(.1f, "loading piggies...");

    this->trashModel = std::make_shared<Model>((char*)"resources/trash/trashbag.obj");
    std::shared_ptr<TrashBag> trashBag = std::make_shared<TrashBag>(*basicShader, *trashModel);
    trashBag->initialize();
    this->addGameObject(trashBag);
    trashBag->addToWorld(this->world);
    progressCallback(.1f, "loading trash...");

    this->kitchenDoorModel = std::make_shared<Model>((char*)"resources/buildings/kitchen/kitchendoors1.obj");
    std::shared_ptr<Door> door = std::make_shared<Door>(*basicShader, *kitchenDoorModel, glm::vec3(8.7, 1.86, 10.42));
    door->initialize();
    this->addGameObject(door);
    door->addToWorld(world);
    progressCallback(.1f, "loading kitchen door...");

    this->kitchenDoorLModel = std::make_shared<Model>((char*)"resources/buildings/kitchen/leftkdoor.obj");
    std::shared_ptr<Door> kitchenDoorL = std::make_shared<Door>(*basicShader, *kitchenDoorLModel, glm::vec3(10.1, 1.86, -3.5), 0,  glm::vec3(0, 1, 0), glm::vec3(0, 0, 1)); // hinge points towards pos z
    kitchenDoorL->initialize();
    this->addGameObject(kitchenDoorL);
    kitchenDoorL->addToWorld(world);
    progressCallback(.1f, "loading kitchen door2...");

    this->kitchenDoorRModel = std::make_shared<Model>((char*)"resources/buildings/kitchen/rightkdoor.obj");
    std::shared_ptr<Door> kitchenDoorR = std::make_shared<Door>(*basicShader, *kitchenDoorRModel, glm::vec3(10.1, 1.86, 3.47), 0, glm::vec3(0, -1, 0), glm::vec3(0, 0, -1));
    kitchenDoorR->initialize();
    this->addGameObject(kitchenDoorR);
    kitchenDoorR->addToWorld(world);
    progressCallback(.1f, "loading kitchen door3...");

    this->dumpsterLidDoorModel = std::make_shared<Model>((char*)"resources/buildings/kitchen/dumpsterliddoor.obj");
    //std::shared_ptr<Door> dumpsterLidDoor = std::make_shared<Door>(*basicShader, *dumpsterLidDoorModel, glm::vec3(-2.5, 4.27, 10.632), 90, glm::vec3(-1, 0, 0));
    std::shared_ptr<Door> dumpsterLidDoor = std::make_shared<Door>(*basicShader, *dumpsterLidDoorModel, glm::vec3(-2.5, 2.27, 20.632), 0, glm::vec3(-1, 0, 0), glm::vec3(0,1,0));
    dumpsterLidDoor->initialize();
    this->addGameObject(dumpsterLidDoor);
    dumpsterLidDoor->addToWorld(world);
    progressCallback(.1f, "loading kitchen door4...");

    // this->arrowsModel = std::make_shared<Model>((char*)"resources/g2.obj");
    // progressCallback(.1f);

    this->kitchenItemsModel = std::make_shared<Model>((char*)"resources/buildings/kitchen/kitchenitems2.obj");
    this->kitchenModel = std::make_shared<Model>((char*)"resources/buildings/kitchen/kitcheninterior3.obj");
    progressCallback(.05f, "loading kitchen...");
    this->terrain = std::make_shared<Terrain>(*kitchenModel);
    terrain->initTerrain();
    terrain->addToWorld(world);
    progressCallback(.05f, "creating kitchen terrain...");

    this->goodcentsModel = std::make_shared<Model>((char*)"resources/characters/siplearm1.fbx");
    this->models.push_back(goodcentsModel);
    this->player = std::make_shared<Player>(camera, this->world, ui, physDebugOn);
    player->initialize();
    this->player->addToWorld(this->world);
    progressCallback(.1f, "initializing player...");
    this->initialized = true;
    std::cout << "DONE" << std::endl;
}

void MainScene::addGameObject(std::shared_ptr<GameObject> gameObject) {
    this->gameObjects.push_back(gameObject);
}

void MainScene::removeGameObject() {

}