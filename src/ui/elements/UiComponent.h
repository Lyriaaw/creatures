//
// Created by Amalric Lombard de Buffières on 10/17/20.
//

#ifndef CREATURES_UICOMPONENT_H
#define CREATURES_UICOMPONENT_H


#include <SFML/Graphics/RectangleShape.hpp>

class UiComponent {
protected:
    double x, y, width, height;
    double xRatio, yRatio, widthRatio, heightRatio;
    double windowWidth, windowHeight;


public:
    UiComponent(double xRatio, double yRatio, double widthRatio, double heightRatio);
    void onWindowResized(double windowWidth, double windowHeight);

    void setFixedSizeAndPosition(double x, double y, double width, double height);
    void mouseMoved(int mouseX, int mouseY, int previousX, int previousY);
    void mouseClicked(int mouseX, int mouseY);


    virtual void mouseHovering(int mouseX, int mouseY) = 0;
    virtual void mouseClickedInside(int mouseX, int mouseY) = 0;
    virtual void resized() = 0;
    virtual void mouseEnter() = 0;
    virtual void mouseLeave() = 0;
    virtual void draw(sf::RenderWindow *window) = 0;

};


#endif //CREATURES_UICOMPONENT_H
