#include "uimaster.hpp"

UIMaster::UIMaster(unsigned int scrWidth, unsigned int scrHeight) : currentDialog(currentDialog) {
    this->scrWidth = scrWidth;
    this->scrHeight = scrHeight;
}

void UIMaster::render(float deltaTime, float curTime) {
    //iterate over elements and render
    for(int i = 0; i < this->textElements.size(); i++) {
        this->textElements[i].render(this->scrWidth / 2, this->scrHeight / 2, 1, glm::vec3(1.0, 0.0, 0.0), curTime);
    }
    // finally show any urgent dialog:
    if(this->dialogShowing) {
        this->currentDialog.render(200, 200, 1, glm::vec3(1.0, 0, 0), deltaTime, glm::vec2(120,100), curTime);
    }
}

void UIMaster::addElement(UIElement newElement) {
    
}

void UIMaster::addTextElement(UITextElement& newElement) {
    this->textElements.push_back(newElement);
}

void UIMaster::showDialog(std::string text) {
    if(this->dialogShowing) {
        // update text of current dialog
        this->currentDialog.updateText(text);
    } else {
        UITextElement textElement("resources/text/Angelic Peace.ttf", text, 48);
        UIElement dialogBox(600, 200, 100, 30, (float)this->scrWidth, (float)this->scrHeight, "resources/crumpleanim");
        this->currentDialog = DialogueElement(textElement, dialogBox);
        this->dialogShowing = true;
        this->currentDialog.activate();
    }
}

void UIMaster::clearDialog() { // add callback to here to reset dialog showing
    this->currentDialog.reset();
}