#ifndef MAINSCENE_HPP
#define MAINSCENE_HPP

#include "scene.hpp"
#include "stb_image.h"
#include "piggy.hpp"

class MainScene : public Scene {
    public:
        MainScene(std::string name);
        void render(float deltaTime, float curTime);
        void initialize(std::function<void(float)> progressCallback);
        void addGameObject(std::shared_ptr<GameObject> gameObject);
        void removeGameObject(); // todo: id system?
    private:
        std::vector<std::shared_ptr<GameObject>> gameObjects;
        std::string name;
        std::shared_ptr<Skybox> skybox;
        btDiscreteDynamicsWorld *world;
        std::shared_ptr<Terrain> terrain;
        std::shared_ptr<Shader> basicShader;
        std::shared_ptr<Model> piggyModel;
        std::shared_ptr<Model> kitchenModel;
        bool initialized;
};

#endif