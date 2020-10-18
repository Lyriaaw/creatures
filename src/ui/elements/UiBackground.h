//
// Created by Amalric Lombard de Buffières on 10/17/20.
//

#ifndef CREATURES_UIBACKGROUND_H
#define CREATURES_UIBACKGROUND_H


#include <SFML/Graphics/RenderWindow.hpp>
#include "UiComponent.h"

class UiBackground: public UiComponent {
private:
    sf::RectangleShape background;
public:
    UiBackground(double xRatio, double yRatio, double widthRatio, double heightRatio);

    void resized() override;

    void mouseEnter() override;

    void mouseLeave() override;

    void draw(sf::RenderWindow *window) override;

    void setFillColor(sf::Color color);

    void mouseHovering(int mouseX, int mouseY) override;

    void mouseClickedInside(int mouseX, int mouseY) override;

};


#endif //CREATURES_UIBACKGROUND_H
