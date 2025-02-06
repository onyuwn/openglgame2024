#ifndef PIGGY_HPP
#define PIGGY_HPP
#define GLM_ENABLE_EXPERIMENTAL
#include <string>
#include <vector>
#include "model.hpp"
#include "shader.hpp"
#include "rigidbodyentity.hpp"
#include "dialogue.hpp"
#include "collisionshapetype.hpp"
#include "gameobject.hpp"
#include "gameobjectinteractiontype.hpp"

class Piggy : public GameObject {
    public:
        std::string name;
        Shader &shader;
        std::vector<std::string> lines;
        bool interacting;
        int lineIdx;
        bool initialized;
        RigidBodyEntity piggyRigidBody;
        glm::mat4 modelMatrix;
        Piggy(std::string name, Shader &shader, Model &piggyModel);
        void render(float deltaTime, glm::mat4 model = glm::mat4(1.0), glm::mat4 view = glm::mat4(1.0), glm::mat4 projection = glm::mat4(1.0), float curTime = 0.0) override;
        GameObjectInteractionType getInteraction() override;
        std::string getDialogueLine() override;
        glm::vec3 getPos();
        void setPos(std::function<glm::vec3()> posCallback) override;
        void initialize() override;
        void addToWorld(btDiscreteDynamicsWorld * world);
        void applyForce(glm::vec3 force) override;
        void toggleRigidBody() override;
        void toggleState() override;
        std::string getHelpText() override;
    private:
        std::function<glm::vec3()> positionCallback;
};

#endif