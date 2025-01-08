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
        DoorRigidBody(Model &doorModel, glm::vec3 doorPos);
        ~DoorRigidBody();
        void initialize();
        void render(Shader &shader, float deltaTime);
        void addToWorld(btDynamicsWorld * world);
        void toggleDoor();
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
};

#endif