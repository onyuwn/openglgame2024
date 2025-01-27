#include "player.hpp"

Player::Player(Camera &camera, btDiscreteDynamicsWorld *world,
               UIMaster &uiCallback, bool &physDebugOn, std::string playerModelPath)
                : camera(camera), uiCallback(uiCallback), physDebugOn(physDebugOn),
                  playerModelPath(playerModelPath), animStart(0.0), curAnim(nullptr) {
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

    this->bonesShader = std::make_shared<Shader>("src/shaders/bones.vs", "src/shaders/basic.fs");
    this->playerModel = std::make_shared<Model>((char*)playerModelPath.data());
    Animation* grabAnim = new Animation(playerModelPath, this->playerModel.get(),1);
    Animation* idleAnim = new Animation(playerModelPath, this->playerModel.get(), 0);
    this->animator = std::make_shared<Animator>(grabAnim);
    this->animator->playAnimation(idleAnim);
    animations.insert({"grab", grabAnim});
    animations.insert({"idle", idleAnim});
}

void Player::render(float curTime, float deltaTime) {
    if(curAnim && playingAnim && curTime - animStart < (curAnim->getDuration() / curAnim->getTicksPerSecond())) {
        this->animator->updateAnimation(deltaTime);
    } else {
        playingAnim = false;
        //curAnim = nullptr;
        curAnim = this->animations["idle"];
        this->animator->playAnimation(curAnim);
        this->animator->updateAnimation(deltaTime);
    }
    glm::mat4 model = glm::translate(glm::mat4(1.0), getPlayerPos() + glm::vec3(0,.75,-.1));
    model = glm::scale(model, glm::vec3(.25, .25, .25));
    model *= glm::mat4(getPlayerRotationMatrix());

    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom * 2.0f), (float)800 / (float)600, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix(getPlayerPos() + glm::vec3(0,1,0));

    auto transforms = this->animator->getFinalBoneMatrices();
    this->bonesShader->use();
    bonesShader->setMat4("projection", projection);
    bonesShader->setMat4("view", view);
    bonesShader->setMat4("model", model);
    for (int i = 0; i < transforms.size(); ++i) {
        this->bonesShader->setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transforms[i]);
    }
    this->playerModel->draw(*this->bonesShader, curTime);
}

void Player::addToWorld(btDiscreteDynamicsWorld * world) {
    world->addRigidBody(this->playerRigidBody);
}

void Player::UpdatePlayer(float curTime, float deltaTime, GLFWwindow *window) {
    processInput(window, curTime, deltaTime);
}

void Player::processInput(GLFWwindow *window, float curTime, float deltaTime)
{ // this should just apply force to the rigid body, translate view matrix with rigid body pos through getter method in the main loop
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    //this->playerRigidBody->activate(true);
    float velocity = (this->camera.MovementSpeed * deltaTime);
    btTransform curTransform = this->playerRigidBody->getWorldTransform();
    btVector3 curPos = curTransform.getOrigin();
    glm::vec3 cameraFrontNormal = glm::normalize(glm::vec3(this->camera.Front.x, 0, this->camera.Front.z));
    glm::vec3 cameraRightNormal = glm::normalize(glm::vec3(this->camera.Right.x, 0, this->camera.Right.z));
    btVector3 btFront = btVector3(cameraFrontNormal.x, 0, cameraFrontNormal.z);
    btVector3 btRight = btVector3(cameraRightNormal.x, 0, cameraRightNormal.z);

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
        this->interact(curTime);
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
    } else { // in the air
        btVector3 currentVelocity = playerRigidBody->getLinearVelocity();
        btVector3 clampedVelocity = currentVelocity;
        float maxSpeed = 3.0f;  // Set the max horizontal speed
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
    }
}

bool Player::checkGrounded() {
    glm::vec3 outOrigin = getPlayerPos();
    glm::vec3 outEnd = outOrigin + this->camera.WorldUp * -3.0f;
    btCollisionWorld::ClosestRayResultCallback RayCallback(btVector3(outOrigin.x, outOrigin.y, outOrigin.z), btVector3(outEnd.x, outEnd.y, outEnd.z));
    world->rayTest(btVector3(outOrigin.x, outOrigin.y, outOrigin.z), btVector3(outEnd.x, outEnd.y, outEnd.z), RayCallback);
    return RayCallback.hasHit();
}

void Player::interact(float curTime) {
    // cast ray, check if entity has interactable content
    glm::vec3 outOrigin = getPlayerPos();
    glm::vec3 outEnd = outOrigin + this->camera.Front * 100.0f;
    btCollisionWorld::ClosestRayResultCallback RayCallback(btVector3(outOrigin.x, outOrigin.y, outOrigin.z), btVector3(outEnd.x, outEnd.y, outEnd.z));
    world->rayTest(btVector3(outOrigin.x, outOrigin.y, outOrigin.z), btVector3(outEnd.x, outEnd.y, outEnd.z), RayCallback);
    if(RayCallback.hasHit() && !itemInHand) {
        GameObject* hitObject = (GameObject*)RayCallback.m_collisionObject->getUserPointer(); // todo cast to generic gameobject
        if(hitObject != nullptr) {
            GameObjectInteractionType interactionType = hitObject->getInteraction();
            // play grab anim once
            curAnim = this->animations["grab"];
            this->animator->playAnimation(curAnim);
            playingAnim = true;
            animStart = curTime;
            printf("animstart: %f", curTime);
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

bool Player::isAlive() {
    return this->getPlayerPos().y > -20.0f;
}