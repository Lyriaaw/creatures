//
// Created by Amalric Lombard de Buffières on 10/17/20.
//

#include "UiBackground.h"

UiBackground::UiBackground(double xRatio, double yRatio, double widthRatio, double heightRatio)
        : UiComponent(xRatio, yRatio, widthRatio, heightRatio) {
    background.setFillColor(sf::Color(25, 50, 50, 255));
}


void UiBackground::resized() {
    background.setSize(sf::Vector2f(width, height));
    background.setPosition(sf::Vector2f(x, y));
}


void UiBackground::mouseEnter() {
}

void UiBackground::mouseLeave() {
}

void UiBackground::draw(sf::RenderWindow *window) {
    window->draw(background);
}

void UiBackground::setFillColor(sf::Color color) {
    background.setFillColor(color);
}

void UiBackground::mouseHovering(int mouseX, int mouseY) {

}

void UiBackground::mouseClickedInside(int mouseX, int mouseY) {

}
