#include "UIElement.hpp"

float timePerFrame = .025;
float timeSinceLastFrame = 0;

// todo accept optional image texture
UIElement::UIElement(int uiWidth, int uiHeight, int xPos, int yPos,
                     float windowWidth, float windowHeight, std::string imagePath) : uiMesh(uiMesh), uiShader(uiShader) {
    this->imagePath = imagePath;
    this->position = glm::vec2(xPos,yPos);
    this->dimensions = glm::vec2(uiWidth,uiHeight);
    this->reverse = false;
    init(uiWidth, uiHeight, xPos, yPos, windowWidth, windowHeight);
}

void UIElement::init(int uiWidth, int uiHeight, int xPos, int yPos, float windowWidth, float windowHeight) {
    this->uiWidth = uiWidth;
    this->uiHeight = uiHeight;
    // init quad mesh
    float topLeftX = xPos;
    float topLeftY = yPos;

    float topRightX = xPos + uiWidth;
    float topRightY = yPos;

    float bottomLeftX = xPos;
    float bottomLeftY = yPos + uiHeight;

    float bottomRightX = xPos + uiWidth;
    float bottomRightY = yPos + uiHeight;

    // std::vector<glm::vec3> vertices = {
    //     glm::vec3(topRightX, topRightY, 0),
    //     glm::vec3(bottomRightX, bottomRightY, 0),
    //     glm::vec3(topLeftX, topLeftY, 0),
    //     glm::vec3(bottomRightX, bottomRightY, 0),
    //     glm::vec3(bottomLeftX, bottomLeftY, 0),
    //     glm::vec3(topLeftX, topLeftY, 0),
    // };

    std::vector<glm::vec3> vertices = {
        glm::vec3(bottomLeftX, bottomLeftY, 0),
        glm::vec3(topLeftX, topLeftY, 0),
        glm::vec3(topRightX, topRightY, 0),
        glm::vec3(bottomLeftX, bottomLeftY, 0),
        glm::vec3(topRightX, topRightY, 0),
        glm::vec3(bottomRightX, bottomRightY, 0),
    };

    if(imagePath.length() > 0) {
        // buffer texture, use different shader?
        std::vector<std::string> imagePaths;
        if(std::__fs::filesystem::is_directory(imagePath)) { // iterate over dir, contains animation frames
            for(const auto &path : std::__fs::filesystem::directory_iterator(imagePath)) {
                imagePaths.push_back(path.path());
            }
        } else {
            imagePaths.push_back(imagePath);
        }

        if(imagePaths.size()  > 1) {
            this->animated = true;
        } else {
            this->animated = false;
        }

        std::sort(imagePaths.begin(), imagePaths.end(), [](const auto& a, const auto& b) {
            return a > b;
        });

        for(int i = 0; i < imagePaths.size(); i++) {
            std::string curPath = imagePaths[i];
            int width, height, nChannels;
            unsigned char *data = stbi_load(curPath.c_str(), &width, &height, &nChannels, 0);
            unsigned int textureID;

            if(data) {
                glGenTextures(1, &textureID);
                glBindTexture(GL_TEXTURE_2D, textureID);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

                Frame frame = {
                    textureID
                };

                frames.push_back(frame);
                stbi_image_free(data);
            }
            else
            {
                std::cout << "UIElement:: Failed to load texture";
            }
        }

        //create tex coords
        // std::vector<glm::vec2> uvs = {
        //     glm::vec2(1.0f, 1.0f),  // Top-right
        //     glm::vec2(1.0f, 0.0f),  // Bottom-right
        //     glm::vec2(0.0f, 1.0f),  // Top-left
        //     glm::vec2(1.0f, 0.0f),  // Bottom-right
        //     glm::vec2(0.0f, 0.0f),  // Bottom-left
        //     glm::vec2(0.0f, 1.0f)   // Top-left
        // };

        std::vector<glm::vec2> uvs = {
            glm::vec2(0.0f, 0.0f),  // Top-right
            glm::vec2(0.0f, 1.0f),  // Bottom-right
            glm::vec2(1.0f, 1.0f),  // Top-left
            glm::vec2(0.0f, 0.0f),  // Bottom-right
            glm::vec2(1.0f, 1.0f),  // Bottom-left
            glm::vec2(1.0f, 0.0f)   // Top-left
        };
        Shader uiShader("src/shaders/uitextureelement.vs", "src/shaders/uitextureelement.fs");
        this->uiShader = uiShader;
        this->uiMesh = UIMesh(vertices, uvs);
    }
    else
    {
        Shader uiShader("src/shaders/uielement.vs", "src/shaders/uielement.fs");
        this->uiShader = uiShader;
        this->uiMesh = UIMesh(vertices);
    }
}

void UIElement::render(float deltaTime, bool &closeCallback, glm::vec4 color) {
    uiShader.use();
    uiShader.setVec4("inColor", color);

    if(this->imagePath.length() > 0) {
        if(frames.size() > 1 && curFrame < frames.size() && !reverse) {
            if(timeSinceLastFrame < timePerFrame) {
                timeSinceLastFrame += deltaTime;
            } else {
                timeSinceLastFrame = 0;
                curFrame++;
            }
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, frames[curFrame].textureID);
            uiShader.setInt("uiTex", 0);
        } else if(reverse && curFrame > 0) {
            if(timeSinceLastFrame < timePerFrame) {
                timeSinceLastFrame += deltaTime;
            } else {
                timeSinceLastFrame = 0;
                curFrame--;
            }
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, frames[curFrame].textureID);
            uiShader.setInt("uiTex", 0);
        } else { // anim complete or static image
            glActiveTexture(GL_TEXTURE0);
            if(frames.size() > 1) {
                glBindTexture(GL_TEXTURE_2D, frames[curFrame - 1].textureID);
                if(animated && reverse) { // hit anim complete callback
                    closeCallback=false;
                    std::cout << "CLOSING DIALOG" << curFrame << std::endl;
                }
            } else {
                glBindTexture(GL_TEXTURE_2D, frames[0].textureID);
            }
            uiShader.setInt("uiTex", 0);
        }
    }

    uiShader.setMat4("projection", glm::ortho(0.0f, 800.0f, 0.0f, 600.0f));
    uiMesh.draw(uiShader);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void UIElement::fadeOut() {
    std::cout << "FADEOUT" << std::endl;
    this->reverse=true;
}

glm::vec2 UIElement::getPos() {
    return this->position;
}

glm::vec2 UIElement::getDims() {
    return this->dimensions;
}