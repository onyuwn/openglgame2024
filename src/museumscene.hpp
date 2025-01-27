#ifndef MUSEUMSCENE_HPP
#define MUSEUMSCENE_HPP
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

class MuseumScene : public Scene {
    public:
        MuseumScene(std::string name, UIMaster &ui, Camera &camera);
        void render(float deltaTime, float curTime, GLFWwindow *window);
        void initialize(std::function<void(float, std::string)> progressCallback);
        void addGameObject(std::shared_ptr<GameObject> gameObject);
        void removeGameObject();
    private:
        std::vector<std::shared_ptr<GameObject>> gameObjects;
        std::vector<std::shared_ptr<Model>> models;
        std::shared_ptr<Model> museumModel;
        std::shared_ptr<Model> museumItemsModel;
        std::shared_ptr<Shader> basicShader;
        std::string name;
        std::shared_ptr<Skybox> skybox;
        btDiscreteDynamicsWorld *world;
        std::shared_ptr<Terrain> terrain;
        std::shared_ptr<Player> player;

        std::shared_ptr<Animator> playerAnimator;

        bool initialized;
        bool physDebugOn;

        Camera &camera;
        UIMaster &ui;
};

#endif