#ifndef TRASHBAG_HPP
#define TRASHBAG_HPP

#include "gameobject.hpp"
#include "rigidbodyentity.hpp"
#include "shader.hpp"
#include "model.hpp"
#include "collisionshapetype.hpp"
#include "gameobjectinteractiontype.hpp"

class TrashBag : public GameObject {
    public:
        TrashBag(Shader &shader, Model &trashBagModel);
        void initialize() override;
        void addToWorld(btDiscreteDynamicsWorld *world);
        void render(float deltaTime, glm::mat4 model = glm::mat4(1.0), glm::mat4 view = glm::mat4(1.0), glm::mat4 projection = glm::mat4(1.0), float curTime = 0.0) override;
        GameObjectInteractionType getInteraction() override;
        std::string getDialogueLine() override;
        void setPos(std::function<glm::vec3()> posCallback) override;
        void applyForce(glm::vec3 force) override;
        void toggleRigidBody() override;
        void toggleState() override;
        std::string getHelpText() override;
        RigidBodyEntity trashBagRigidBody;
    private:
        Shader &shader;
        Shader *outlineShader;
        Model &trashBagModel;
        bool initialized;
        bool positionOverride;
        std::function<glm::vec3()> positionCallback;
};

#endif