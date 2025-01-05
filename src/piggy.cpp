#include "piggy.hpp"

Piggy::Piggy(std::string name, Shader &shader) : name(name), interacting(false), shader(shader),
                                                 piggyRigidBody(Model("resources/piggyiso.obj"), btVector3(1,2,-2), BOX) {
    std::vector<std::string> linesTemp
    {
        "it's so\nover",
        "it never\nstarted",
        "I'll never be satisfied",
        "i can wait",
        "it won't hit it hits\nit won't hit"
    };
    this->lines = linesTemp;
    this->lineIdx = -1;
    this->initialized = false;

    glm::mat4 model = glm::mat4(1.0);
    model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    this->modelMatrix = model;
}

void Piggy::initialize() {
    this->piggyRigidBody.initialize(this->modelMatrix);
    this->initialized = true;
}

void Piggy::addToWorld(btDynamicsWorld * world) {
    if(initialized) {
        this->piggyRigidBody.addToWorld(world);
        this->piggyRigidBody.entityRigidBody->setUserPointer(this);
    }
}

void Piggy::render(float deltaTime) {
    piggyRigidBody.render(this->shader, glm::mat4(1.0));
}

std::string Piggy::getInteraction() {
    lineIdx++;
    if(lineIdx < lines.size()) {
        return this->lines[this->lineIdx];
    } else {
        return "\0";
    }
}

glm::vec3 Piggy::getPos() {
    return this->piggyRigidBody.getPos();
}