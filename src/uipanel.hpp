#ifndef UIPANEL_HPP
#define UIPANEL_HPP

#include "UIElement.hpp"
#include "UIMesh.hpp"
#include "shader.hpp"
#include <glm/glm.hpp>
#include <vector>

class UIPanel : public UIElement {
    public:
        UIPanel(int width, int height, int xPos, int yPos, float windowWidth, float windowHeight, glm::vec4 backgroundColor);
        void render(float deltaTime) override;
        void init(int width, int height, int xPos, int yPos, float windowWidth, float windowHeight) override;
        glm::vec2 getPos() override;
        glm::vec2 getDims() override;
        void updateWindowSize(int newWindowWidth, int newWindowHeight) override;
    private:
        int width;
        int height;
        int xPos;
        int yPos;
        int windowWidth;
        int windowHeight;
        glm::vec4 backgroundColor;
        std::shared_ptr<Shader> uiPanelShader;
        std::shared_ptr<UIMesh> uiMesh;
};

#endif