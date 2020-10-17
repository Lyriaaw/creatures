//
// Created by Amalric Lombard de Buffières on 10/17/20.
//

#ifndef CREATURES_UILABEL_H
#define CREATURES_UILABEL_H


#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include "UiComponent.h"

class UiLabel: public UiComponent {
private:
    sf::Text text;
public:
    UiLabel(double xRatio, double yRatio, double widthRatio, double heightRatio);

    void resized() override;

    void mouseEnter() override;

    void mouseLeave() override;

    void draw(sf::RenderWindow *window) override;

    void setFillColor(sf::Color color);

    void mouseHovering(int mouseX, int mouseY) override;

    void mouseClickedInside(int mouseX, int mouseY) override;

    void setText(std::string textString);

    void setCharacterSize(int characterSize);
};


#endif //CREATURES_UILABEL_H
