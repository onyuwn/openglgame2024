#include "doorrigidbody.hpp"

DoorRigidBody::DoorRigidBody(Model &doorModel, glm::vec3 doorPos, float initialRot, glm::vec3 rotAxis, glm::vec3 doorForward) 
    : doorModel(doorModel), initialized(false), isOpen(false), rotating(false),
      meshOrigin(glm::vec3(0.0, 0.0, 0.0)), doorPos(doorPos), doorSize(glm::vec3(1.0)),
      doorHingeConstraint(nullptr), initialRot(initialRot), rotAxis(rotAxis), doorForward(doorForward) {}

DoorRigidBody::~DoorRigidBody() {
    if (this->doorRigidBody) {
        delete this->doorRigidBody->getMotionState();
        delete this->doorRigidBody;
    }
    if (this->doorCollisionShape) {
        delete this->doorRigidBody;
    }
}

void DoorRigidBody::initialize() {
    glm::vec3 minVertex(FLT_MAX, FLT_MAX, FLT_MAX);
    glm::vec3 maxVertex(-FLT_MAX, -FLT_MAX, -FLT_MAX);

    std::vector<Mesh> entityMeshes = this->doorModel.getMeshes();  // Get mesh data
    // Iterate through all vertices in the mesh to calculate the bounding box
    for (int h = 0; h < entityMeshes.size(); h++) {
        Mesh terrainMesh = entityMeshes[h];
        
        for (int i = 0; i < terrainMesh.vertices.size(); i++) {
            glm::vec3 vertex = terrainMesh.vertices[i].Position;
            // Update the bounding box limits
            minVertex = glm::min(minVertex, vertex);
            maxVertex = glm::max(maxVertex, vertex);
        }
    }

    this->meshOrigin = (minVertex + maxVertex) * 0.5f;
    printf("meshorigin: %f, %f, %f\n", meshOrigin.x, meshOrigin.y, meshOrigin.z);
    printf("doorforward: %f, %f, %f\n", doorForward.x, doorForward.y, doorForward.z);
    // Calculate the size (width, height, depth) of the bounding box
    glm::vec3 size = maxVertex - minVertex;
    this->doorSize = size;
    printf("size: %f, %f, %f\n", size.x, size.y, size.z);
    // Create the btBoxShape using the half-extents
    btVector3 halfExtents(size.x * 0.5f, size.y * 0.5f, size.z * 0.5f);
    btBoxShape* boxShape = new btBoxShape(halfExtents);
    this->doorCollisionShape = boxShape;

    btTransform startTransform;
    btVector3 axis = btVector3(rotAxis.x, rotAxis.y, rotAxis.z);
    btQuaternion initQuat = btQuaternion(axis, glm::radians(initialRot));
    btVector3 offset = btVector3(size.x * doorForward.x * .5, size.y * doorForward.y * .5, size.z * doorForward.z * .5);
    startTransform.setIdentity();
    startTransform.setOrigin(btVector3(doorPos.x, doorPos.y, doorPos.z) + offset);
    doorAngle = initialRot;
    btDefaultMotionState* motionstate = new btDefaultMotionState(startTransform);

    btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(
        0,
        motionstate,
        this->doorCollisionShape,
        btVector3(0,0,0)
    );
    this->doorRigidBody = new btRigidBody(rigidBodyCI);
    this->doorRigidBody->activate(true);

    this->initialized = true;
}

void DoorRigidBody::addToWorld(btDynamicsWorld * world) {
    if(initialized) {
        world->addRigidBody(this->doorRigidBody);
        //world->addConstraint(this->doorHingeConstraint);
        this->doorRigidBody->setActivationState(1);
        this->doorRigidBody->activate(true);
    }
}

void DoorRigidBody::toggleDoor() {
    this->isOpen = !this->isOpen;
    this->rotating = true;
}

void DoorRigidBody::render(Shader &shader, float deltaTime) {
    shader.use();
    glm::mat4 modelRotationMatrix = glm::translate(glm::mat4(1.0f), this->doorPos);
    if (rotating) {
        // Calculate direction (1 for opening, -1 for closing)
        float direction = this->isOpen ? 1.0f : -1.0f;
        doorAngle += 180.0f * direction * deltaTime;
        btTransform currentTransform = this->doorRigidBody->getWorldTransform();

        // this is retarded but fuck it I've spent too much time here
        if(doorForward.x != 0) {
            if (isOpen && doorAngle >= 90.0f) { // TODO: figure out how to use hinge constraint?
                int sign = 1;
                if(rotAxis.x != 0) { sign = rotAxis.x; }
                if(rotAxis.y != 0) { sign = rotAxis.y; }
                if(rotAxis.z != 0) { sign = rotAxis.z; }
                std::cout << "open" << std::endl;
                doorAngle = 90.0f;
                rotating = false;
                btVector3 offset = btVector3(doorSize.z * doorForward.z * .5, doorSize.y * doorForward.y * .5, doorSize.x * doorForward.x * .5);
                currentTransform.setOrigin(btVector3(doorPos.x, doorPos.y, doorPos.z) - (offset * sign));
                //doorRigidBody->setCollisionFlags(doorRigidBody->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
            } else if (!isOpen && doorAngle <= 0.0f) {
                int sign = 1;
                if(rotAxis.x != 0) { sign = rotAxis.x; }
                if(rotAxis.y != 0) { sign = rotAxis.y; }
                if(rotAxis.z != 0) { sign = rotAxis.z; }
                std::cout << "closed" << std::endl;
                doorAngle = 0.0f;
                rotating = false;
                btVector3 offset = btVector3(doorSize.x * doorForward.x * .5, doorSize.y * doorForward.y * .5, doorSize.z * doorForward.z * .5);
                currentTransform.setOrigin(btVector3(doorPos.x, doorPos.y, doorPos.z) + offset);
                //doorRigidBody->setCollisionFlags(doorRigidBody->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT);
            }
        } else if(doorForward.z != 0) {
            if (isOpen && doorAngle >= 90.0f) { // TODO: figure out how to use hinge constraint?
                int sign = 1;
                if(rotAxis.x != 0) { sign = rotAxis.x; }
                if(rotAxis.y != 0) { sign = rotAxis.y; }
                if(rotAxis.z != 0) { sign = rotAxis.z; }
                std::cout << "open" << std::endl;
                doorAngle = 90.0f;
                rotating = false;
                btVector3 offset = btVector3(doorSize.z * doorForward.z * .5, doorSize.y * doorForward.y * .5, doorSize.x * doorForward.x * .5);
                currentTransform.setOrigin(btVector3(doorPos.x, doorPos.y, doorPos.z) + (offset * sign));
                //doorRigidBody->setCollisionFlags(doorRigidBody->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
            } else if (!isOpen && doorAngle <= 0.0f) {
                int sign = 1;
                if(rotAxis.x != 0) { sign = rotAxis.x; }
                if(rotAxis.y != 0) { sign = rotAxis.y; }
                if(rotAxis.z != 0) { sign = rotAxis.z; }
                std::cout << "closed" << std::endl;
                doorAngle = 0.0f;
                rotating = false;
                btVector3 offset = btVector3(doorSize.x * doorForward.x * .5, doorSize.y * doorForward.y * .5, doorSize.z * doorForward.z * .5);
                currentTransform.setOrigin(btVector3(doorPos.x, doorPos.y, doorPos.z) + offset);
                //doorRigidBody->setCollisionFlags(doorRigidBody->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT);
            }
        } else if(doorForward.y != 0) {
            if (isOpen && doorAngle >= 90.0f) { // TODO: figure out how to use hinge constraint?
                int sign = 1;
                if(rotAxis.x != 0) { sign = rotAxis.x; }
                if(rotAxis.y != 0) { sign = rotAxis.y; }
                if(rotAxis.z != 0) { sign = rotAxis.z; }
                std::cout << "open" << std::endl;
                doorAngle = 90.0f;
                rotating = false;
                btVector3 offset = btVector3(doorSize.x * doorForward.x * .5, doorSize.y * doorForward.y * .5, doorSize.z * doorForward.z * .5);
                currentTransform.setOrigin(btVector3(doorPos.x, doorPos.y, doorPos.z) + (offset * sign));
                //doorRigidBody->setCollisionFlags(doorRigidBody->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
            } else if (!isOpen && doorAngle <= 0.0f) {
                int sign = 1;
                if(rotAxis.x != 0) { sign = rotAxis.x; }
                if(rotAxis.y != 0) { sign = rotAxis.y; }
                if(rotAxis.z != 0) { sign = rotAxis.z; }
                std::cout << "closed" << std::endl;
                doorAngle = 0.0f;
                rotating = false;
                btVector3 offset = btVector3(doorSize.x * doorForward.x * .5, doorSize.y * doorForward.y * .5, doorSize.z * doorForward.z * .5);
                currentTransform.setOrigin(btVector3(doorPos.x, doorPos.y, doorPos.z) + offset);
                //doorRigidBody->setCollisionFlags(doorRigidBody->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT);
            }
        }

        glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(doorAngle), rotAxis);
        glm::quat rotationQuat = glm::quat_cast(rotationMatrix);
        btQuaternion btRot(rotationQuat.x, rotationQuat.y, rotationQuat.z, rotationQuat.w);
        currentTransform.setRotation(btRot);
        this->doorRigidBody->setWorldTransform(currentTransform);
    }
    modelRotationMatrix *= glm::rotate(glm::mat4(1.0f), glm::radians(doorAngle), rotAxis);
    shader.setMat4("model", modelRotationMatrix);
    this->doorModel.draw(shader);
}