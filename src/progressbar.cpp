#include "progressbar.hpp"

ProgressBar::ProgressBar() {
    init();
}

void ProgressBar::init() {
    float topLeftX = 100;
    float topLeftY = 30;

    float topRightX = 100 + 600;
    float topRightY = 30;

    float bottomLeftX = 100;
    float bottomLeftY = 30 + 25;

    float bottomRightX = 100 + 600;
    float bottomRightY = 30 + 25;

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
    this->progressBarShader = std::make_shared<Shader>("src/shaders/uitextureelement.vs", "src/shaders/uiprogressbar.fs");
    this->uiMesh = std::make_shared<UIMesh>(vertices, uvs);
}

void ProgressBar::update(float progress) {
    this->progressBarShader->use();
    this->progressBarShader->setFloat("progress", progress);
    this->progressBarShader->setMat4("projection", glm::ortho(0.0f, 800.0f, 0.0f, 600.0f));
    this->uiMesh->draw(*this->progressBarShader);
}