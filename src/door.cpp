#include "door.hpp"

Door::Door(Shader &shader, Model &doorModel, glm::vec3 doorPos, float initialRot, glm::vec3 rotAxis, glm::vec3 doorForward) : shader(shader), doorModel(doorModel),
                             doorForward(doorForward),
                             doorRigidBody(doorModel, doorPos, initialRot, rotAxis, doorForward),
                             initialized(false)
{
    std::cout << "door" << std::endl;
    this->outlineShader = new Shader("src/shaders/outline.vs", "src/shaders/outline.fs");
}

void Door::render(float deltaTime, glm::mat4 model, glm::mat4 view, glm::mat4 projection, float curTime) {
    if(selected) {
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glEnable(GL_STENCIL_TEST);
        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        glStencilMask(0x00);
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilMask(0xFF);
    }
    this->doorRigidBody.render(this->shader, deltaTime);
    // render outline model here separately from rigid body?
    if(selected) {
        glEnable(GL_CULL_FACE);
        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        glStencilMask(0x00);
        //glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        this->outlineShader->use();
        glm::mat4 finalDoorMatrix = glm::scale(this->doorRigidBody.getDoorModelMatrix(), glm::vec3(1.05, 1.05, 1.05));
        finalDoorMatrix = glm::translate(finalDoorMatrix, glm::vec3(-doorForward.x *.1, 0, -doorForward.z * .1));
        //glm::mat4 finalDoorMatrix = this->doorRigidBody.getDoorModelMatrix();
        this->outlineShader->setMat4("model", finalDoorMatrix);
        this->outlineShader->setMat4("view", view);
        this->outlineShader->setMat4("projection", projection);
        this->outlineShader->setFloat("curTime", curTime);
        this->doorModel.drawOutline(*this->outlineShader);
        glDisable(GL_BLEND);
        glStencilMask(0xFF);
        glStencilFunc(GL_ALWAYS, 0, 0xFF);
        glEnable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);  
    }
    selected = false;
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

std::string Door::getHelpText() {
    return "open";
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
