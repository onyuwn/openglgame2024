#include "door.hpp"

Door::Door() : doorRigidBody(doorRigidBody) {}

void Door::render(float deltaTime) {

}

void Door::initialize() {

}

GameObjectInteractionType Door::getInteraction() {

}

std::string Door::getDialogueLine() {

}

void Door::setPos(std::function<glm::vec3()> posCallback) {

}

void Door::applyForce(glm::vec3 force) {

}

void Door::toggleRigidBody() {

}

void Door::open(float deltaTime, float curTime) {

}

void Door::addToWorld(btDynamicsWorld * world) {
    // add hinge constraint
}

