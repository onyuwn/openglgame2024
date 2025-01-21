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
#include <glm/gtc/type_ptr.hpp>

class MainScene : public Scene {
    public:
        MainScene(std::string name, UIMaster &ui, Camera &camera);
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
        std::shared_ptr<Terrain> terrain;
        std::shared_ptr<Shader> basicShader;
        std::shared_ptr<Model> piggyModel;
        std::shared_ptr<Model> kitchenModel;
        std::shared_ptr<Model> kitchenItemsModel;
        std::shared_ptr<Model> kitchenDoorModel;
        std::shared_ptr<Model> kitchenDoorRModel;
        std::shared_ptr<Model> kitchenDoorLModel;
        std::shared_ptr<Model> dumpsterLidDoorModel;
        std::shared_ptr<Model> deskModel;
        std::shared_ptr<Model> trashModel;
        std::shared_ptr<Model> arrowsModel;
        std::shared_ptr<Model> gModel;

        std::shared_ptr<Animation> testAnim;
        std::shared_ptr<Animator> animator;
        std::shared_ptr<Model> goodcentsModel;

        std::shared_ptr<Player> player;
        std::shared_ptr<DebugDrawer> debugDrawer;

        //Player &player;
        Camera &camera;
        UIMaster &ui;

        bool initialized;
        bool physDebugOn;
};

#endif