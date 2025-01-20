#include "player.hpp"

Player::Player(Camera &camera, btDiscreteDynamicsWorld *world, UIMaster &uiCallback, bool &physDebugOn) : camera(camera), uiCallback(uiCallback), physDebugOn(physDebugOn) {
    this->camera = camera;
    this->world = world;
        // build collision shape (box for rn)
    this->playerCollisionShape = new btCapsuleShape(1.0, 2.0);
    // create rigid body
    this->initialized = false;
    this->uiCallback = uiCallback;
}

void Player::initialize() {
    glm::quat defaultQuat = glm::quat(0,0,0,0);
    glm::vec3 defaultPos = glm::vec3(3,0,0);

    btTransform startTransform;
    startTransform.setIdentity();
    startTransform.setOrigin(btVector3(2, 2, 2));

    btDefaultMotionState* motionstate = new btDefaultMotionState(startTransform);

    btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(
        10,
        motionstate,
        this->playerCollisionShape,
        btVector3(0,0,0)
    );
    this->playerRigidBody = new btRigidBody(rigidBodyCI);
    this->playerRigidBody->activate(true);
    this->playerRigidBody->setFriction(.5);
    this->playerRigidBody->setDamping(0.6f, 0.9f);
    this->initialized=true;
}

void Player::addToWorld(btDiscreteDynamicsWorld * world) {
    world->addRigidBody(this->playerRigidBody);
}

void Player::UpdatePlayer(float deltaTime, GLFWwindow *window) {
    processInput(window, deltaTime);
}

void Player::processInput(GLFWwindow *window, float deltaTime)
{ // this should just apply force to the rigid body, translate view matrix with rigid body pos through getter method in the main loop
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    //this->playerRigidBody->activate(true);
    float velocity = (this->camera.MovementSpeed * deltaTime);
    btTransform curTransform = this->playerRigidBody->getWorldTransform();
    btVector3 curPos = curTransform.getOrigin();
    btVector3 btFront = btVector3(this->camera.Front.x, this->camera.Front.y, this->camera.Front.z);
    btVector3 btRight = btVector3(this->camera.Right.x, this->camera.Right.y, this->camera.Right.z);

    btVector3 force(0, 0, 0);  // Reset force each frame

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        this->playerRigidBody->setActivationState(1);
        this->playerRigidBody->activate(true);
        force += btFront * velocity * 10000;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        this->playerRigidBody->setActivationState(1);
        this->playerRigidBody->activate(true);
        force += btFront * velocity * -10000;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        this->playerRigidBody->setActivationState(1);
        this->playerRigidBody->activate(true);
        force += btRight * velocity * -10000;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        this->playerRigidBody->setActivationState(1);
        this->playerRigidBody->activate(true);
        force += btRight * velocity * 10000;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && checkGrounded()) {
        this->playerRigidBody->setActivationState(1);
        this->playerRigidBody->activate(true);
        force += btVector3(0, 1, 0) * velocity * 50000;
    }
    if(glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS && !interactRequested) {
        this->interact();
        interactRequested = true;
    }
    if(glfwGetKey(window, GLFW_KEY_E) == GLFW_RELEASE) {
        interactRequested = false;
    }
    if(glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
        physDebugOn = true;
    } else {
        physDebugOn = false;
    }

    // Apply force smoothly
    if (force.length() != 0 && checkGrounded()) {
        btVector3 currentVelocity = playerRigidBody->getLinearVelocity();
        btVector3 clampedVelocity = currentVelocity;
        float maxSpeed = 10.0f;  // Set the max horizontal speed
        if (clampedVelocity.x() > maxSpeed) {
            clampedVelocity.setX(maxSpeed);
        }
        if (clampedVelocity.x() < -maxSpeed) {
            clampedVelocity.setX(-maxSpeed);
        }
        if (clampedVelocity.z() > maxSpeed) {
            clampedVelocity.setZ(maxSpeed);
        }
        if (clampedVelocity.z() < -maxSpeed) {
            clampedVelocity.setZ(-maxSpeed);
        }

        // Set the vertical velocity (y) without clamping it
        clampedVelocity.setY(currentVelocity.y());

        // Apply the clamped velocity back to the rigid body (only affects horizontal speed)
        playerRigidBody->setLinearVelocity(clampedVelocity);
        this->playerRigidBody->applyCentralForce(force);
    } else if(checkGrounded()) {
        btVector3 currentVelocity = playerRigidBody->getLinearVelocity();
        btScalar dampingFactor = 0.98f;  // Damping factor for smooth deceleration
        playerRigidBody->setLinearVelocity(currentVelocity * dampingFactor);
    }
}

bool Player::checkGrounded() {
    glm::vec3 outOrigin = getPlayerPos();
    glm::vec3 outEnd = outOrigin + this->camera.Up * -3.0f;
    btCollisionWorld::ClosestRayResultCallback RayCallback(btVector3(outOrigin.x, outOrigin.y, outOrigin.z), btVector3(outEnd.x, outEnd.y, outEnd.z));
    world->rayTest(btVector3(outOrigin.x, outOrigin.y, outOrigin.z), btVector3(outEnd.x, outEnd.y, outEnd.z), RayCallback);
    return RayCallback.hasHit();
}

void Player::interact() {
    // cast ray, check if entity has interactable content
    glm::vec3 outOrigin = getPlayerPos();
    glm::vec3 outEnd = outOrigin + this->camera.Front * 100.0f;
    btCollisionWorld::ClosestRayResultCallback RayCallback(btVector3(outOrigin.x, outOrigin.y, outOrigin.z), btVector3(outEnd.x, outEnd.y, outEnd.z));
    world->rayTest(btVector3(outOrigin.x, outOrigin.y, outOrigin.z), btVector3(outEnd.x, outEnd.y, outEnd.z), RayCallback);
    if(RayCallback.hasHit() && !itemInHand) {
        GameObject* hitObject = (GameObject*)RayCallback.m_collisionObject->getUserPointer(); // todo cast to generic gameobject
        if(hitObject != nullptr) {
            GameObjectInteractionType interactionType = hitObject->getInteraction();

            if(interactionType == DIALOGUE) {
                // if interaction returns something....
                std::string dialogLine = hitObject->getDialogueLine();
                hitObject->setPos([this]() { return getPlayerPos(); });
                if(dialogLine == "\0") {
                    this->uiCallback.clearDialog();
                } else {
                    std::cout << dialogLine << std::endl;
                    this->uiCallback.showDialog(dialogLine);
                }
            } else if(interactionType == HOLD_ITEM) {
                hitObject->setPos([this]() { return getPlayerHandPos(); });
                this->itemInHand = true;
                this->heldItem = hitObject;
            } else if(interactionType == THROW_ITEM) {
                hitObject->applyForce(getPlayerHandPos()); // need to reactivate the rigid body of the interactive entity
            } else if(interactionType == TOGGLE) {
                std::cout << "TOGGLE REQUESTED" << std::endl;
                hitObject->toggleState();
            }
        } else {
            std::cout <<"NADA\n";
        }
    } else if(itemInHand) { // throw that shit
        this->heldItem->toggleRigidBody();
        glm::vec3 throwingForce = this->camera.Front * 5.0f + glm::vec3(0,5,0);
        this->heldItem->applyForce(throwingForce);
        // apply force
        this->heldItem = nullptr;
        itemInHand = false;
    }
}

glm::vec3 Player::getPlayerPos() {
    btTransform curTransform = this->playerRigidBody->getWorldTransform();
    btVector3 curPos = curTransform.getOrigin();
    //btQuaternion curRot = curTransform.getRotation();
    return glm::vec3(curPos.x(), curPos.y(), curPos.z());
}

glm::vec3 Player::getPlayerHandPos() {
    return getPlayerPos() + this->camera.Front * 2.0f;
}

glm::mat3 Player::getPlayerRotationMatrix() {
    return glm::mat3(this->camera.Right, this->camera.Up, this->camera.Front);
}