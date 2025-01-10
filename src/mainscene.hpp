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

class MainScene : public Scene {
    public:
        MainScene(std::string name, UIMaster &ui, Camera &camera);
        void render(float deltaTime, float curTime, GLFWwindow *window);
        void initialize(std::function<void(float)> progressCallback);
        void addGameObject(std::shared_ptr<GameObject> gameObject);
        void removeGameObject(); // todo: id system?
    private:
        std::vector<std::shared_ptr<GameObject>> gameObjects;
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
        std::shared_ptr<Model> deskModel;
        std::shared_ptr<Model> trashModel;
        std::shared_ptr<Model> arrowsModel;
        std::shared_ptr<Model> gModel;

        std::shared_ptr<Player> player;

        //Player &player;
        Camera &camera;
        UIMaster &ui;

        bool initialized;
};

#endif