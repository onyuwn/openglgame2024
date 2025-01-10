#ifndef UIMASTER_HPP
#define UIMASTER_HPP

#include <vector>
#include "UIElement.hpp"
#include "uitextelement.hpp"
#include "dialogue.hpp"
#include <string>

class UIMaster {
    public:
        UIMaster(unsigned int scrWidth, unsigned int scrHeight);
        void render(float deltaTime, float curTime);
        void showSceneLoadProgress(float progress);
        void addElement(UIElement newElement);
        void addTextElement(UITextElement& newElement);
        void showDialog(std::string text);
        void clearDialog();
    private:
        unsigned int scrWidth;
        unsigned int scrHeight;
        std::vector<UIElement> elements;
        std::vector<UITextElement> textElements;
        DialogueElement currentDialog;
        bool dialogShowing;
};

#endif