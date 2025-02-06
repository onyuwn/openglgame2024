#include "trashbag.hpp"

TrashBag::TrashBag(Shader &shader, Model &trashBagModel) : shader(shader),
                                     trashBagRigidBody(trashBagModel, btVector3(10,2,1)),
                                     trashBagModel(trashBagModel) {
    this->initialized = false;
    this->positionOverride = false;
    this->outlineShader = new Shader("src/shaders/outline.vs", "src/shaders/outline.fs");
}

void TrashBag::initialize() {
    this->trashBagRigidBody.initialize();
    this->initialized = true;
}

void TrashBag::addToWorld(btDiscreteDynamicsWorld *world) {
    if(this->initialized) {
        this->trashBagRigidBody.addToWorld(world);
        this->trashBagRigidBody.entityRigidBody->setUserPointer(this);
    }
}

void TrashBag::render(float deltaTime, glm::mat4 model, glm::mat4 view, glm::mat4 projection, float curTime) {
    if(selected) {
        std::cout << "trash selected" << std::endl;
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glEnable(GL_STENCIL_TEST);
        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        glStencilMask(0x00);
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilMask(0xFF);
    }

    if(this->positionOverride) {
        glm::mat4 model = glm::translate(glm::mat4(1.0), this->positionCallback());
        model = glm::scale(model, glm::vec3(.5,.5,.5));
        this->trashBagRigidBody.render(this->shader, model, true);
    } else {
        this->trashBagRigidBody.render(this->shader);
    }

    if(selected && !this->positionOverride) {
        glEnable(GL_CULL_FACE);
        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        glStencilMask(0x00);
        this->outlineShader->use();
        glm::mat4 finalTrashBagModelMatrix = glm::scale(this->trashBagRigidBody.getFinalModelMatrix(), glm::vec3(1.15, 1.15, 1.15));
        //glm::mat4 finalDoorMatrix = this->doorRigidBody.getDoorModelMatrix();
        this->outlineShader->setMat4("model", finalTrashBagModelMatrix);
        this->outlineShader->setMat4("view", view);
        this->outlineShader->setMat4("projection", projection);
        this->outlineShader->setFloat("curTime", curTime);
        this->trashBagModel.drawOutline(*this->outlineShader);
        glStencilMask(0xFF);
        glStencilFunc(GL_ALWAYS, 0, 0xFF);
        glEnable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
    }
    selected = false;
}

std::string TrashBag::getDialogueLine() {
    return "";
}

void TrashBag::setPos(std::function<glm::vec3()> posCallback) {
    this->positionCallback = posCallback;
}

GameObjectInteractionType TrashBag::getInteraction() {
    if(this->positionOverride) {
        this->positionOverride = false;
        return THROW_ITEM;
    } else {
        this->positionOverride = true;
        return HOLD_ITEM;
    }
}

void TrashBag::applyForce(glm::vec3 force) {
    if(this->positionOverride) {
        std::cout << "applying force" << std::endl;
        this->positionOverride = false;
        this->trashBagRigidBody.entityRigidBody->setActivationState(1);
        this->trashBagRigidBody.entityRigidBody->activate(true);
    }
    btVector3 btForce = btVector3(force.x, force.y, force.z);
    this->trashBagRigidBody.entityRigidBody->setCollisionFlags(0);
    this->trashBagRigidBody.entityRigidBody->applyCentralImpulse(btForce);
}

void TrashBag::toggleRigidBody() {
    std::cout << "toggline rigid body" << std::endl;
    glm::vec3 initPos = this->positionCallback();
    this->trashBagRigidBody.setPos(initPos);
    this->trashBagRigidBody.entityRigidBody->activate(true);
    this->positionOverride = false;
    std::cout << "rigidbody toggled" << std::endl;
}

void TrashBag::toggleState() {}

std::string TrashBag::getHelpText() {

}