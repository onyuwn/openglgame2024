// doors are modeled offset from the world origin. they don't move, only rotate
#ifndef DOORRIGIDBODY_HPP
#define DOORRIGIDBODY_HPP
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"
#include "model.hpp"
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include "shader.hpp"


class DoorRigidBody {
    public:
        DoorRigidBody(Model &doorModel, glm::vec3 doorPos, float initialRot = 0, glm::vec3 rotAxis = glm::vec3(0, 1.0, 0), glm::vec3 doorForward = glm::vec3(-1, 0, 0));
        ~DoorRigidBody();
        void initialize();
        void render(Shader &shader, float deltaTime, bool selected = false);
        void addToWorld(btDynamicsWorld * world);
        void toggleDoor();
        glm::mat4 getDoorModelMatrix();
        btRigidBody *doorRigidBody;
    private:
        Model &doorModel;
        btCollisionShape *doorCollisionShape;
        btHingeConstraint *doorHingeConstraint;
        bool initialized;
        bool isOpen;
        bool rotating;
        float doorAngle;
        glm::vec3 meshOrigin;
        glm::vec3 doorPos;
        glm::vec3 doorSize;
        float initialRot;
        glm::vec3 rotAxis;
        glm::vec3 doorForward;
        glm::mat4 finalDoorModelMatrix;
};

#endif