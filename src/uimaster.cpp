#include "uimaster.hpp"

UIMaster::UIMaster(unsigned int scrWidth, unsigned int scrHeight) {
    this->scrWidth = scrWidth;
    this->scrHeight = scrHeight;
    this->pauseMenuPanel = std::make_shared<UIPanel>(scrWidth, scrHeight, 0, 0, scrWidth, scrHeight, glm::vec4(1.0,0.0,0.0,.5));
}

void UIMaster::render(float deltaTime, float curTime) {
    //iterate over elements and render
    for(int i = 0; i < this->textElements.size(); i++) {
        this->textElements[i]->render(this->scrWidth / 2, this->scrHeight / 2, 1, glm::vec3(1.0, 0.0, 0.0), curTime);
    }
    // finally show any urgent dialog:
    if(this->dialogShowing) {
        this->currentDialog->render(200, 200, 1, glm::vec3(1.0, 0, 0), deltaTime, glm::vec2(120,100), curTime);
    }

    if(gamePaused && this->pauseMenuPanel) { // show menu
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        this->pauseMenuPanel->render(deltaTime);
    }
}

void UIMaster::showSceneLoadProgress(float progress) {
    
}

void UIMaster::addElement(UIElement* newElement) {
    
}

void UIMaster::addTextElement(UITextElement *newElement) {
    this->textElements.push_back(newElement);
}

void UIMaster::showDialog(std::string text) {
    if(this->dialogShowing) {
        // update text of current dialog
        this->currentDialog->updateText(text);
    } else {
        this->currentDialog = std::make_shared<DialogueElement>();
        this->dialogShowing = true;
        this->currentDialog->activate();
    }
}

void UIMaster::clearDialog() { // add callback to here to reset dialog showing
    this->currentDialog->reset();
}