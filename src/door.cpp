#include "door.hpp"

Door::Door(Shader &shader, Model &doorModel, glm::vec3 doorPos, float initialRot, glm::vec3 rotAxis, glm::vec3 doorForward) : shader(shader),
                             doorRigidBody(doorModel, doorPos, initialRot, rotAxis, doorForward),
                             initialized(false)
{
    std::cout << "door" << std::endl;
}

void Door::render(float deltaTime) {
    this->doorRigidBody.render(this->shader, deltaTime);
}

void Door::initialize() {
    this->doorRigidBody.initialize();
    this->initialized = true;
}

GameObjectInteractionType Door::getInteraction() {
    return TOGGLE;
}

std::string Door::getDialogueLine() {
    return "";
}

void Door::setPos(std::function<glm::vec3()> posCallback) { }

void Door::applyForce(glm::vec3 force) { }

void Door::toggleRigidBody() { }

void Door::toggleHinge(float deltaTime, float curTime) {
    if(this->isOpen) {

    } else {

    }
    this->isOpen = !this->isOpen;
}

void Door::addToWorld(btDynamicsWorld * world) {
    // add hinge constraint
    if(initialized) {
        this->doorRigidBody.addToWorld(world);
        this->doorRigidBody.doorRigidBody->setUserPointer(this);
    }
}

void Door::toggleState() {
    this->doorRigidBody.toggleDoor();
}
