#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"
#include "model.hpp"
#include "camera.hpp"
#include <glm/gtc/quaternion.hpp>
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include "rigidbodyentity.hpp"
#include "piggy.hpp"
#include "uimaster.hpp"
#include "gameobject.hpp"
#include "gameobjectinteractiontype.hpp"

class Player { // todo: inherit rigid body entity
    public:
        btCollisionShape* playerCollisionShape;
        btRigidBody* playerRigidBody;
        glm::vec3 position;

        Player(Camera &camera, btDiscreteDynamicsWorld *world, UIMaster &uiCallback); // todo: add player model
        void initialize();
        void UpdatePlayer(float deltaTime, GLFWwindow *window);
        void processInput(GLFWwindow *window, float deltaTime);
        void addToWorld(btDiscreteDynamicsWorld * world);
        void interact();
        bool checkGrounded();
        glm::vec3 getPlayerPos();
        glm::vec3 getPlayerHandPos();
    private:
        Camera &camera;
        btDiscreteDynamicsWorld *world;
        bool interactRequested;
        UIMaster &uiCallback;
        bool initialized;
        GameObject* heldItem;
        bool itemInHand;
};

#endif