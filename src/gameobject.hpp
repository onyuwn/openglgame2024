#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

#include <glm/glm.hpp>
#include <string>
#include <functional>
#include "gameobjectinteractiontype.hpp"

class GameObject {
    public:
        virtual void render(float deltaTime) = 0;
        virtual void initialize() = 0;
        virtual GameObjectInteractionType getInteraction() = 0;
        virtual std::string getDialogueLine() = 0;
        //maybe rename this to overridepos
        virtual void setPos(std::function<glm::vec3()> posCallback) = 0;
        virtual void applyForce(glm::vec3 force) = 0;
        virtual void toggleRigidBody() = 0;
        //virtual void toggleState() = 0; // door
};

#endif