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
        void render(float deltaTime) override;
        GameObjectInteractionType getInteraction() override;
        std::string getDialogueLine() override;
        void setPos(std::function<glm::vec3()> posCallback) override;
        void applyForce(glm::vec3 force) override;
        void toggleRigidBody() override;
        void toggleState() override;
        RigidBodyEntity trashBagRigidBody;
    private:
        Shader &shader;
        bool initialized;
        bool positionOverride;
        std::function<glm::vec3()> positionCallback;
};

#endif