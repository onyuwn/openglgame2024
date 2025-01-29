#include "piggy.hpp"

Piggy::Piggy(std::string name, Shader &shader, Model &piggyModel) : name(name), interacting(false), shader(shader),
                                                 piggyRigidBody(piggyModel, btVector3(0, 2, 18), BOX, 10.0f) {
    std::vector<std::string> linesTemp
    {
        "it's so\nover",
        "it never\nstarted",
        "I'll never be satisfied",
        "i can wait",
        "it won't hit it hits\nit won't hit",
        "\0"
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

void Piggy::addToWorld(btDiscreteDynamicsWorld *world) {
    if(initialized) {
        this->piggyRigidBody.addToWorld(world);
        this->piggyRigidBody.entityRigidBody->setUserPointer(this);
    }
}

void Piggy::render(float deltaTime) {
    if(this->interacting) { // position override
        glm::vec3 direction = glm::normalize(this->positionCallback() - this->getPos());
        glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0), this->getPos());
        glm::vec3 currentForward = glm::vec3(0.0f, 0.0f, 1.0f);
        glm::vec3 axis = glm::cross(currentForward, direction);
        float angle = acos(glm::dot(currentForward, direction));
        if (glm::length(axis) > 0.0001f) {
            axis = glm::normalize(axis);
            glm::mat4 rotationMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0,axis.y,0)); // important. probably a better way to do this with lookat
            modelMatrix = rotationMatrix * modelMatrix;
        }
        modelMatrix = glm::translate(modelMatrix, -this->getPos());
        piggyRigidBody.render(this->shader, modelMatrix, true);
    } else {
        piggyRigidBody.render(this->shader, glm::mat4(1.0));
    }
}

GameObjectInteractionType Piggy::getInteraction() {
    this->interacting = true;
    return DIALOGUE;
}

std::string Piggy::getDialogueLine() {
    lineIdx++;
    if(lineIdx < lines.size()) {
        return this->lines[this->lineIdx];
    } else {
        this->interacting = false;
        return "\0";
    }
}

void Piggy::setPos(std::function<glm::vec3()> posCallback) {
    this->positionCallback = posCallback; // used to look at player
}

glm::vec3 Piggy::getPos() {
    return this->piggyRigidBody.getPos();
}

void Piggy::applyForce(glm::vec3 force) {
    // not needed
}

void Piggy::toggleRigidBody() {
    // not needed
}

void Piggy::toggleState() {
    
}