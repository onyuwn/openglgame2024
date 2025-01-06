#ifndef TRASHBAG_HPP
#define TRASHBAG_HPP

#include "gameobject.hpp"

class TrashBag : public GameObject {
    public:
        void render(float deltaTime) override;
        std::string getInteraction() override;
        void initialize() override;
};

#endif