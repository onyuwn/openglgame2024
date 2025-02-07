#include "postprocessor.hpp"

PostProcessor::PostProcessor() {
    initialize();
}

void PostProcessor::initialize() {
    std::cout << "postprocessor initialized." << std::endl;
    glGenFramebuffers(1, &frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    // create a color attachment texture
    glGenTextures(1, &textureColorBuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorBuffer, 0);

    glGenRenderbuffers(1, &renderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBuffer);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    std::vector<glm::vec3> vertices = {
        glm::vec3(-1.0, 1.0, 0),
        glm::vec3(-1.0, -1.0, 0),
        glm::vec3(1.0, -1.0, 0),
        glm::vec3(-1.0, 1.0, 0),
        glm::vec3(1.0, -1.0, 0),
        glm::vec3(1.0, 1.0, 0),
    };

    std::vector<glm::vec2> uvs = {
        glm::vec2(0.0f, 1.0f),
        glm::vec2(0.0f, 0.0f),
        glm::vec2(1.0f, 0.0f),
        glm::vec2(0.0f, 1.0f),
        glm::vec2(1.0f, 0.0f),
        glm::vec2(1.0f, 1.0f) 
    };

    this->quad = new UIMesh(vertices, uvs);
    this->screenShader = new Shader("src/shaders/screenshader.vs", "src/shaders/screenshader.fs");
    this->screenShader->use();
    this->screenShader->setInt("screenTexture",0);

    this->initialized = true;
}

void PostProcessor::begin() {
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    glEnable(GL_DEPTH_TEST);
}

void PostProcessor::render() { // split up first and second render passes somehow -- another method?
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST);
    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    this->screenShader->use();
    glBindTexture(GL_TEXTURE_2D, textureColorBuffer);
    this->quad->draw(*this->screenShader);
}