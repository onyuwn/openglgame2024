#ifndef DIALOGUE_HPP
#define DIALOGUE_HPP
#include "uitextelement.hpp"
#include "UIElement.hpp"
#include <vector>
#include <string>
#include <glm/glm.hpp>

class DialogueElement {
    public:
        DialogueElement(UITextElement textElement, UIElement dialogBox);
        void activate();
        void advance();
        void reset();
        void render(float x, float y, float scale, glm::vec3 color, float deltaTime, glm::vec2 padding, float curTime);
        void updateText(std::string newText);
        bool showing;
    private:
        std::vector<std::string> lines;
        UITextElement textElement;
        UIElement dialogBox;
        unsigned int lineIdx;
};

#endif