#ifndef DOOR_HPP
#define DOOR_HPP

#include "gameobject.hpp"
#include "gameobjectinteractiontype.hpp"
#include "doorrigidbody.hpp"
#include "model.hpp"
#include "shader.hpp"
#include "model.hpp"
#include <string>

class Door : public GameObject { // use hinge constraint
    public:
        Door(Shader &shader, Model &doorModel, glm::vec3 doorPos);
        void render(float deltaTime) override;
        void initialize() override;
        GameObjectInteractionType getInteraction() override;
        virtual std::string getDialogueLine() override;
        virtual void setPos(std::function<glm::vec3()> posCallback) override;
        virtual void applyForce(glm::vec3 force) override;
        virtual void toggleRigidBody() override;
        void toggleHinge(float deltaTime, float curTime);
        void addToWorld(btDynamicsWorld * world);
        void toggleState() override;
    private:
        Shader &shader;
        bool isOpen;
        bool initialized;
        DoorRigidBody doorRigidBody;

};

#endif