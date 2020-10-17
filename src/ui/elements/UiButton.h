//
// Created by Amalric Lombard de Buffières on 10/17/20.
//

#ifndef CREATURES_UIBUTTON_H
#define CREATURES_UIBUTTON_H


#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include "UiComponent.h"

class UiButton: public UiComponent {
private:
    sf::RectangleShape background;
    sf::Text text;
public:
    UiButton(const std::string& textString, double xRatio, double yRatio, double widthRatio, double heightRatio);

    void mouseHovering(int mouseX, int mouseY) override;

    void mouseClickedInside(int mouseX, int mouseY) override;

    void resized() override;

    void mouseEnter() override;

    void mouseLeave() override;

    void draw(sf::RenderWindow *window) override;
};


#endif //CREATURES_UIBUTTON_H
