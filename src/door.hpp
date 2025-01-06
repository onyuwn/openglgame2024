#ifndef DOOR_HPP
#define DOOR_HPP

#include "gameobject.hpp"
#include <string>

class Door : public GameObject {
    public:
        Door();
        void render(float deltaTime) override;
        std::string getInteraction() override;
        void initialize() override;
        void open(float deltaTime, float curTime);
        bool open;
}

#endif