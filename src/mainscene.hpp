#ifndef MAINSCENE_HPP
#define MAINSCENE_HPP

#include "scene.hpp"
#include "stb_image.h"
#include "piggy.hpp"
#include "trashbag.hpp"
#include "door.hpp"
#include "billboardentity.hpp"
#include "player.hpp"
#include "uimaster.hpp"
#include "debugdrawer.hpp"
#include "animation.hpp"
#include "animator.hpp"
#include "postprocessor.hpp"
#include "billboardentity.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <sstream>

class MainScene : public Scene {
    public:
        MainScene(std::string name, UIMaster &ui, Camera &camera, std::function<void(std::string)> changeSceneCallback);
        void render(float deltaTime, float curTime, GLFWwindow *window);
        void initialize(std::function<void(float, std::string)> progressCallback);
        void addGameObject(std::shared_ptr<GameObject> gameObject);
        void removeGameObject(); // todo: id system?
    private:
        std::vector<std::shared_ptr<GameObject>> gameObjects;
        std::vector<std::shared_ptr<Model>> models;
        std::string name;
        std::shared_ptr<Skybox> skybox;
        //std::unique_ptr<btDiscreteDynamicsWorld> world;
        btDiscreteDynamicsWorld * world;
        std::shared_ptr<Terrain> restaurantTerrain;
        std::shared_ptr<Shader> basicShader;
        std::shared_ptr<Shader> bonesShader;
        std::shared_ptr<Shader> waterShader;
        std::shared_ptr<Model> trashModel;
        std::shared_ptr<Model> restaurantModel;
        std::shared_ptr<Model> restaurantItemsModel;
        std::shared_ptr<Model> restaurantFacadeModel;
        //std::shared_ptr<Model> restaurantExtModel;
        std::shared_ptr<Model> bossGuyBillboardModel;
        std::shared_ptr<Model> bushBillboardModel;
        std::shared_ptr<Model> waterModel;
        std::shared_ptr<BillboardEntity>  bossGuyBillboard;
        std::shared_ptr<BillboardEntity>  bushBillboard;
        std::shared_ptr<UITextElement> playerPosTxt;

        std::shared_ptr<PostProcessor> postProcessor;

        Animation *testAnim;
        std::shared_ptr<Animator> animator;

        std::shared_ptr<Player> player;
        std::shared_ptr<DebugDrawer> debugDrawer;

        //Player &player;
        Camera &camera;
        UIMaster &ui;

        bool initialized;
        bool paused;
        bool physDebugOn;

        std::function<void(std::string)> changeSceneCallback;
};

#endif