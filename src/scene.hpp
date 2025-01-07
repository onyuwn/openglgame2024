#ifndef SCENE_HPP
#define SCENE_HPP
#include "gameobject.hpp"
#include <vector>
#include <string>

class Scene {
    public:
        virtual void render(std::string sceneName, float deltaTime, float curTime) = 0;
        virtual void initialize(float &progressCallback) = 0;
        virtual void addGameObject(GameObject gameObject) = 0;
        virtual void removeGameObject() = 0; // todo: id system?
    private:
        std::vector<GameObject> gameObjects;
        std::string name;
        //dynamics world
        //add to world

};

#endif