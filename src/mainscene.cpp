#include "mainscene.hpp"

MainScene::MainScene(std::string name, UIMaster &ui, Camera &camera, std::function<void(std::string)> changeSceneCallback) : initialized(false),
                                                                       ui(ui), camera(camera), physDebugOn(false) {
    this->changeSceneCallback = changeSceneCallback;
}

void MainScene::render(float deltaTime, float curTime, GLFWwindow *window) {
    if(this->initialized && this->player->isAlive()) {
        this->world->stepSimulation(deltaTime * 3.0f, 7);
        this->player->UpdatePlayer(curTime, deltaTime, window, paused);
        this->animator->updateAnimation(deltaTime);
        glm::vec3 playerPos = this->player->getPlayerPos();
        std::stringstream ppStream;
        ppStream << playerPos.x << ", " << playerPos.y << ", " << playerPos.z;
        this->playerPosTxt->setText(ppStream.str());

        this->camera.controlsDisabled = this->player->isControlDisabled();
        this->ui.gamePaused = paused;

        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom * 2.0f), (float)800 / (float)600, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix(player->getPlayerPos() + glm::vec3(0,1,0));
        //glm::mat4 view = camera.GetViewMatrix(glm::vec3(0,0,0));

        this->basicShader->use();
        basicShader->setMat4("projection", projection);
        basicShader->setMat4("view", view);
        glm::mat4 model = glm::mat4(1.0f);

        basicShader->setMat4("model", model);
        this->kitchenItemsModel->draw(*this->basicShader);

        for(int i = 0; i < this->models.size(); i++) {}

        auto transforms = this->animator->getFinalBoneMatrices();
        this->bonesShader->use();
        bonesShader->setMat4("projection", projection);
        bonesShader->setMat4("view", view);
        model = glm::translate(model, glm::vec3(3, 0, 20));
        bonesShader->setMat4("model", model);
        for (int i = 0; i < transforms.size(); ++i) {
            this->bonesShader->setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transforms[i]);
        }
        this->carrotModel->draw(*this->bonesShader, curTime);

        this->player->render(curTime, deltaTime);
        this->basicShader->use();
        this->restaurantItemsModel->draw(*this->basicShader);
        this->officeItemsModel->draw(*this->basicShader);
        this->terrain->render(*this->basicShader);
        this->desertTerrain->render(*this->basicShader);
        this->officeTerrain->render(*this->basicShader);
        this->restaurantTerrain->render(*this->basicShader);

        for(int i = 0; i < this->gameObjects.size(); i++) {
            this->gameObjects[i]->render(deltaTime, model, view, projection, curTime);
        }

        this->skybox->render(glm::mat4(glm::mat3(view)), projection);

        if(this->physDebugOn) {
            debugDrawer->SetMatrices(view, projection);
            world->debugDrawWorld();
        }
        this->ui.render(deltaTime, curTime);

        glfwSwapBuffers(window);
        glfwPollEvents();
    } else if (!this->player->isAlive()) {
        std::cout << "DEAD" << std::endl;
        this->changeSceneCallback("museum");
    }
}

void MainScene::initialize(std::function<void(float, std::string)> progressCallback) {
    this->basicShader = std::make_shared<Shader>("src/shaders/basic.vs", "src/shaders/basic.fs");
    this->bonesShader = std::make_shared<Shader>("src/shaders/bones.vs", "src/shaders/basic.fs");

    std::vector<std::string> skyboxFaces = { // rgba
        "resources/mainskybox/gradiesn.png",
        "resources/mainskybox/gradiesn.png",
        "resources/mainskybox/py.png",
        "resources/mainskybox/ny.png",
        "resources/mainskybox/gradiesn.png",
        "resources/mainskybox/gradiesn.png",
    };
    this->skybox = std::make_shared<Skybox>(skyboxFaces);
    progressCallback(.05f, "creating skybox");
    btBroadphaseInterface* broadphase = new btDbvtBroadphase();
    btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
    btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
    btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;
    this->world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);

    this->debugDrawer = std::make_shared<DebugDrawer>();
    debugDrawer->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
    world->setDebugDrawer(debugDrawer.get());

    progressCallback(.05f, "initializing physics...");
    stbi_set_flip_vertically_on_load(false);
    world->setGravity(btVector3(0,-9.81f,0));

    this->piggyModel = std::make_shared<Model>((char*)"resources/piggyiso.obj");
    std::shared_ptr<Piggy> piggy = std::make_shared<Piggy>("Piggy", *basicShader, *piggyModel);
    piggy->initialize();
    this->addGameObject(piggy);
    piggy->addToWorld(this->world);
    progressCallback(.05f, "loading piggies...");
    progressCallback(.05f, "hi :_)...");

    this->trashModel = std::make_shared<Model>((char*)"resources/trash/trashbag.obj");
    std::shared_ptr<TrashBag> trashBag = std::make_shared<TrashBag>(*basicShader, *trashModel);
    trashBag->initialize();
    this->addGameObject(trashBag);
    trashBag->addToWorld(this->world);
    progressCallback(.05f, "loading trash...");
    progressCallback(.05f, "mygofd...");

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

    this->desertTerrainModel = std::make_shared<Model>((char*)"resources/buildings/kitchen/kitchendesertterrain.obj");
    this->desertTerrain = std::make_shared<Terrain>(*desertTerrainModel);
    desertTerrain->initTerrain();
    desertTerrain->addToWorld(world);
    progressCallback(.05f, "creating desert terrain...");

    this->carrotModel = std::make_shared<Model>((char*)"resources/characters/carrot4.gltf");
    this->testAnim = new Animation("resources/characters/carrot4.gltf", this->carrotModel.get());
    this->animator = std::make_shared<Animator>(testAnim);

    this->player = std::make_shared<Player>(camera, this->world, ui, physDebugOn, "resources/characters/arms4.gltf");
    player->initialize();
    this->player->addToWorld(this->world);
    progressCallback(.1f, "initializing player...");

    this->playerPosTxt = std::make_shared<UITextElement>("resources/text/Angelic Peace.ttf", "X", 48, 400, 10);
    this->ui.addTextElement(this->playerPosTxt.get());

    this->restaurantModel = std::make_shared<Model>((char*)"resources/buildings/restaurant/restaurant.obj");
    this->restaurantItemsModel = std::make_shared<Model>((char*)"resources/buildings/restaurant/restaurantitems.obj");
    this->restaurantTerrain = std::make_shared<Terrain>(*this->restaurantModel);
    this->restaurantTerrain->initTerrain();
    this->restaurantTerrain->addToWorld(world);

    this->officeModel = std::make_shared<Model>((char*)"resources/buildings/office/office.obj");
    this->officeItemsModel = std::make_shared<Model>((char*)"resources/buildings/office/officeitems.obj");
    this->officeTerrain = std::make_shared<Terrain>(*this->officeModel);
    this->officeTerrain->initTerrain();
    this->officeTerrain->addToWorld(world);

    this->initialized = true;
    std::cout << "DONE" << std::endl;
}

void MainScene::addGameObject(std::shared_ptr<GameObject> gameObject) {
    this->gameObjects.push_back(gameObject);
}

void MainScene::removeGameObject() {

}