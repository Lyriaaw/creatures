//
// Created by Amalric Lombard de Buffières on 10/17/20.
//

#include <SFML/Graphics/RenderWindow.hpp>
#include "UiComponent.h"


UiComponent::UiComponent(double xRatio, double yRatio, double widthRatio, double heightRatio) {
    background = sf::RectangleShape(sf::Vector2f(width, height));
    background.setFillColor(sf::Color(50, 50, 50, 255));

    this->xRatio = xRatio;
    this->yRatio = yRatio;
    this->widthRatio = widthRatio;
    this->heightRatio = heightRatio;
}

void UiComponent::setFixedSizeAndPosition(double x, double y, double width, double height) {
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
}

void UiComponent::onWindowResized(double windowWidth, double windowHeight) {
    if (xRatio != 0) {
        x = xRatio * windowWidth;
    }
    if (yRatio != 0) {
        y = yRatio * windowHeight;
    }

    if (widthRatio != 0) {
        width = widthRatio * windowWidth;
    }
    if (heightRatio != 0) {
        height = heightRatio * windowHeight;
    }

    background.setSize(sf::Vector2f(width, height));
    background.setPosition(sf::Vector2f(x, y));
}

void UiComponent::mouseMoved(int mouseX, int mouseY) {
    if (mouseX < x || mouseY < y || mouseX > x + width || mouseY > y + height) {
        background.setFillColor(sf::Color(50, 50, 50, 255));
        return;
    }

    background.setFillColor(sf::Color(100, 100, 100, 255));
    mouseHovering(mouseX, mouseY);
}


void UiComponent::mouseClicked(int mouseX, int mouseY) {
    if (mouseX < x || mouseY < y || mouseX > x + width || mouseY > y + height) {
        return;
    }
    mouseClickedInside(mouseX, mouseY);
}

void UiComponent::mouseClickedInside(int mouseX, int mouseY) {

}
void UiComponent::mouseHovering(int mouseX, int mouseY) {

}



void UiComponent::draw(sf::RenderWindow *window) {
    window->draw(background);
}