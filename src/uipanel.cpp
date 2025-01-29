#include "uipanel.hpp"

UIPanel::UIPanel(int width, int height, int xPos, int yPos, float windowWidth, float windowHeight, glm::vec4 backgroundColor) :
    width(width), height(height), xPos(xPos), yPos(yPos), windowWidth(windowWidth), windowHeight(windowHeight), backgroundColor(backgroundColor)
{
    init(width, height, xPos, yPos, windowWidth, windowHeight);
}

void UIPanel::init(int width, int height, int xPos, int yPos, float windowWidth, float windowHeight) {
    float topLeftX = xPos;
    float topLeftY = yPos;

    float topRightX = xPos + width;
    float topRightY = yPos;

    float bottomLeftX = xPos;
    float bottomLeftY = yPos + height;

    float bottomRightX = xPos + width;
    float bottomRightY = yPos + height;

    std::vector<glm::vec3> vertices = {
        glm::vec3(bottomLeftX, bottomLeftY, 0),
        glm::vec3(topLeftX, topLeftY, 0),
        glm::vec3(topRightX, topRightY, 0),
        glm::vec3(bottomLeftX, bottomLeftY, 0),
        glm::vec3(topRightX, topRightY, 0),
        glm::vec3(bottomRightX, bottomRightY, 0),
    };

    std::vector<glm::vec2> uvs = {
        glm::vec2(0.0f, 0.0f),  // Top-right
        glm::vec2(0.0f, 1.0f),  // Bottom-right
        glm::vec2(1.0f, 1.0f),  // Top-left
        glm::vec2(0.0f, 0.0f),  // Bottom-right
        glm::vec2(1.0f, 1.0f),  // Bottom-left
        glm::vec2(1.0f, 0.0f)   // Top-left
    };

    this->uiMesh = std::make_shared<UIMesh>(vertices, uvs);
    this->uiPanelShader = std::make_shared<Shader>("src/shaders/uielement.vs", "src/shaders/uielement.fs");
}

void UIPanel::render(float deltaTime) {
    this->uiPanelShader->use();
    this->uiPanelShader->setMat4("projection", glm::ortho(0.0f, 800.0f, 0.0f, 600.0f));
    this->uiPanelShader->setVec4("inColor", this->backgroundColor);
    uiMesh->draw(*uiPanelShader);
}

glm::vec2 UIPanel::getPos() {
    return glm::vec2(windowWidth, windowHeight);
}

glm::vec2 UIPanel::getDims() {
    return glm::vec2(width, height);
}

void UIPanel::updateWindowSize(int newWindowWidth, int newWindowHeight) {

}
