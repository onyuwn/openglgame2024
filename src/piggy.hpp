#ifndef PIGGY_HPP
#define PIGGY_HPP
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include "model.hpp"
#include "shader.hpp"
#include "rigidbodyentity.hpp"
#include "dialogue.hpp"
#include "UIElement.hpp"
#include "collisionshapetype.hpp"
#include "gameobject.hpp"

class Piggy : public GameObject {
    public:
        std::string name;
        Shader shader;
        std::vector<std::string> lines;
        bool interacting;
        int lineIdx;
        bool initialized;
        RigidBodyEntity piggyRigidBody;
        glm::mat4 modelMatrix;
        Piggy(std::string name, Shader &shader);
        void render(float deltaTime) override;
        std::string getInteraction() override;
        glm::vec3 getPos();
        void initialize() override;
        void addToWorld(btDynamicsWorld * world);
};

#endif