#ifndef MAINSCENE_HPP
#define MAINSCENE_HPP

#include "scene.hpp"

class MainScene : public Scene {
    public:
        void render(std::string sceneName, float deltaTime, float curTime);
        void initialize(float &progressCallback);
        void addGameObject(GameObject gameObject);
        void removeGameObject(); // todo: id system?
    private:
        std::vector<GameObject> gameObjects;
        std::string name;
        bool initialized;
};

#endif