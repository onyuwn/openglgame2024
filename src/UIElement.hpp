#ifndef UI_HPP
#define UI_HPP

#include "mesh.hpp"
#include "shader.hpp"
#include "UIMesh.hpp"
#include "stb_image.h"
#include <filesystem>
#include <iostream>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>

class UIElement { // maybe need to make ui mesh class but we will see. maybe just change buffer type in shader to vec3
    public:
        UIElement(int uiWidth, int uiHeight, int xPos, int yPos, float windowWidth, float windowHeight, std::string imagePath = "");
        void render(float deltaTime, bool &closeCallback, glm::vec4 color = glm::vec4(1.0, 0.0, 0.0, 1.0));
        void init(int uiWidth, int uiHeight, int xPos, int yPos, float windowWidth, float windowHeight);
        void fadeOut();
        glm::vec2 getPos();
        glm::vec2 getDims();
        struct Frame {
            unsigned int textureID;
        };
        bool animated;
        bool reverse;
    private:
        int uiWidth;
        int uiHeight;
        int curFrame = 0;
        UIMesh uiMesh;
        Shader uiShader;
        std::string imagePath;
        std::vector<Frame> frames;
        glm::vec2 position;
        glm::vec2 dimensions;
};

#endif