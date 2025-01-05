#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

#include <string>

class GameObject {
    public:
        virtual void render(float deltaTime) = 0;
        virtual void initialize() = 0;
        virtual std::string getInteraction() = 0;
};

#endif