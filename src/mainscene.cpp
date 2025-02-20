#include "mainscene.hpp"

MainScene::MainScene(std::string name, UIMaster &ui, Camera &camera, std::function<void(std::string)> changeSceneCallback) : initialized(false),
                                                                       ui(ui), camera(camera), physDebugOn(false) {
    this->changeSceneCallback = changeSceneCallback;
}

void MainScene::render(float deltaTime, float curTime, GLFWwindow *window) {
    if(this->initialized && this->player->isAlive()) {
        this->postProcessor->begin();

        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        this->world->stepSimulation(deltaTime * 3.0f, 7);
        this->player->UpdatePlayer(curTime, deltaTime, window, paused);
        glm::vec3 playerPos = this->player->getPlayerPos();
        std::stringstream ppStream;
        ppStream << playerPos.x << ", " << playerPos.y << ", " << playerPos.z;
        this->playerPosTxt->setPos(glm::vec2(800 - (this->playerPosTxt->getDims().x), 570));
        this->playerPosTxt->setText(ppStream.str());

        this->camera.controlsDisabled = this->player->isControlDisabled();
        this->ui.gamePaused = paused;

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom * 2.0f), (float)800 / (float)600, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix(player->getPlayerPos());

        this->player->render(curTime, deltaTime);
        glm::mat4 model = glm::mat4(1.0f);
        this->waterShader->use();
        waterShader->setMat4("projection", projection);
        waterShader->setMat4("view", view);
        waterShader->setFloat("time", curTime);
        waterShader->setVec3("playerPos", this->player->getPlayerPos());
        model = glm::translate(model, glm::vec3(-5.0, 2.0, 0.0));
        model = glm::scale(model, glm::vec3(10.0, 10.0, 10.0));
        waterShader->setMat4("model", model);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        this->waterModel->draw(*this->waterShader);

        this->basicShader->use();
        basicShader->setMat4("projection", projection);
        basicShader->setMat4("view", view);
        basicShader->setMat4("model", model);
        this->basicShader->setVec3("lightPos", this->player->getPlayerPos());
        this->basicShader->setVec3("lightColor", glm::vec3(1.0, 1.0, 1.0));
        this->restaurantItemsModel->draw(*this->basicShader);
        //this->restaurantExtModel->draw(*this->basicShader);
        this->restaurantFacadeModel->draw(*this->basicShader);
        this->restaurantTerrain->render(*this->basicShader);

        for(int i = 0; i < this->gameObjects.size(); i++) {
            this->gameObjects[i]->render(deltaTime, model, view, projection, curTime);
        }

        this->skybox->render(glm::mat4(glm::mat3(view)), projection);

        if(this->physDebugOn) {
            debugDrawer->SetMatrices(view, projection);
            world->debugDrawWorld();
        }

        this->bossGuyBillboard->render(view, projection, this->camera.Up, this->camera.Right, this->player->getPlayerPos());
        this->bushBillboard->render(view, projection, this->camera.Up, this->camera.Right, this->player->getPlayerPos());

        this->postProcessor->render();

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
    this->waterShader = std::make_shared<Shader>("src/shaders/water.vs", "src/shaders/water.fs");

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

    this->postProcessor = std::make_shared<PostProcessor>();

    this->debugDrawer = std::make_shared<DebugDrawer>();
    debugDrawer->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
    world->setDebugDrawer(debugDrawer.get());

    progressCallback(.05f, "initializing physics...");
    stbi_set_flip_vertically_on_load(false);
    world->setGravity(btVector3(0,-9.81f,0));

    this->trashModel = std::make_shared<Model>((char*)"resources/trash/trashbag.obj");
    std::shared_ptr<TrashBag> trashBag = std::make_shared<TrashBag>(*basicShader, *trashModel);
    trashBag->initialize();
    this->addGameObject(trashBag);
    trashBag->addToWorld(this->world);
    progressCallback(.05f, "loading trash...");
    progressCallback(.05f, "mygofd...");

    progressCallback(.05f, "initializing player...");
    this->player = std::make_shared<Player>(camera, this->world, ui, physDebugOn, "resources/characters/arms4.gltf");
    player->initialize();
    this->player->addToWorld(this->world);

    this->playerPosTxt = std::make_shared<UITextElement>("resources/text/Angelic Peace.ttf", "X", 24, 400, 10);
    this->ui.addTextElement(this->playerPosTxt.get());
    progressCallback(.05f, "initializing restaurant...");
    this->restaurantModel = std::make_shared<Model>((char*)"resources/buildings/restaurantv2/restaurant.obj");
    progressCallback(.05f, "initializing restaurant items...");
    this->restaurantItemsModel = std::make_shared<Model>((char*)"resources/buildings/restaurantv2/restaurantitems.obj");
    this->restaurantFacadeModel = std::make_shared<Model>((char*)"resources/buildings/restaurantv2/restaurantfacade.obj");
    //this->restaurantExtModel = std::make_shared<Model>((char*)"resources/buildings/restaurantv2/restaurantext.obj");
    this->bushBillboardModel = std::make_shared<Model>((char*)"resources/buildings/bush2.obj");
    this->waterModel = std::make_shared<Model>((char*)"resources/buildings/water.obj");
    this->bushBillboard = std::make_shared<BillboardEntity>(*this->bushBillboardModel, glm::vec3(10, 3, 10), glm::vec3(3.0, 3.0, 3.0));
    this->restaurantTerrain = std::make_shared<Terrain>(*this->restaurantModel);
    this->restaurantTerrain->initTerrain();
    this->restaurantTerrain->addToWorld(world);

    this->bossGuyBillboardModel = std::make_shared<Model>((char*)"resources/buildings/office/officeboss.obj");
    this->bossGuyBillboard = std::make_shared<BillboardEntity>(*this->bossGuyBillboardModel, glm::vec3(0,3,0), glm::vec3(3.0, 3.0, 3.0));

    this->initialized = true;
    std::cout << "DONE" << std::endl;
}

void MainScene::addGameObject(std::shared_ptr<GameObject> gameObject) {
    this->gameObjects.push_back(gameObject);
}

void MainScene::removeGameObject() {

}