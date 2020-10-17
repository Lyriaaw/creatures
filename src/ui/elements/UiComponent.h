//
// Created by Amalric Lombard de Buffières on 10/17/20.
//

#ifndef CREATURES_UICOMPONENT_H
#define CREATURES_UICOMPONENT_H


#include <SFML/Graphics/RectangleShape.hpp>

class UiComponent {
protected:
    UiComponent();

    double x, y, width, height;
    double xRatio, yRatio, widthRatio, heightRatio;
    double windowWidth, windowHeight;

    sf::RectangleShape background;


public:
    UiComponent(double xRatio, double yRatio, double widthRatio, double heightRatio);
    void onWindowResized(double windowWidth, double windowHeight);

    void draw(sf::RenderWindow *window);

    void setFixedSizeAndPosition(double x, double y, double width, double height);

    void mouseMoved(int mouseX, int mouseY);

    void mouseHovering(int mouseX, int mouseY);

    void mouseClicked(int mouseX, int mouseY);

    void mouseClickedInside(int mouseX, int mouseY);
};


#endif //CREATURES_UICOMPONENT_H
