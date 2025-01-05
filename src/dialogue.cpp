#include "dialogue.hpp"

DialogueElement::DialogueElement(UITextElement textElement, UIElement dialogBox) : textElement(textElement), dialogBox(dialogBox) {
    this->textElement = textElement;
    this->lineIdx = 0;
    this->showing = false;
    this->dialogBox = dialogBox;
}

void DialogueElement::activate() {
    // render first line
    std::cout << "dialogactivated\n";
    this->showing = true;
    // init dialogbox
}

void DialogueElement::render(float x, float y, float scale, glm::vec3 color, float deltaTime, glm::vec2 padding, float curTime) { // will have to come up with some sort of binary to encode all this in easily
    if(showing) {
        std::cout << "rendering dialog\n";
        this->dialogBox.render(deltaTime, showing);
        this->textElement.render(this->dialogBox.getPos().x + padding.x, (this->dialogBox.getPos().y + this->dialogBox.getDims().y) - padding.y, scale, color, curTime);
    }
}

void DialogueElement::updateText(std::string newText) { // todo: add args for font family and size (with defaults)
    this->textElement = UITextElement("resources/text/Angelic Peace.ttf", newText, 48);
}

void DialogueElement::advance() {
    // update text element to display next line
}

void DialogueElement::reset() {
    std::cout << "dialog reset" << std::endl;
    this->lineIdx = 0;
    if(this->dialogBox.animated) {
        // run anim in reverse then stop rendering
        this->dialogBox.fadeOut();
    } else {
        this->showing = false;
    }
}