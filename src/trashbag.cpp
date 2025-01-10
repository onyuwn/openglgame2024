#include "trashbag.hpp"

TrashBag::TrashBag(Shader &shader, Model &trashBagModel) : shader(shader),
                                     trashBagRigidBody(trashBagModel, btVector3(10,2,1)) {
    this->initialized = false;
    this->positionOverride = false;
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

void TrashBag::render(float deltaTime) {
    if(this->positionOverride) {
        glm::mat4 model = glm::translate(glm::mat4(1.0), this->positionCallback());
        model = glm::scale(model, glm::vec3(.5,.5,.5));
        this->trashBagRigidBody.render(this->shader, model, true);
    } else {
        this->trashBagRigidBody.render(this->shader);
    }
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