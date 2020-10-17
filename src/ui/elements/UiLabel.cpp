//
// Created by Amalric Lombard de Buffières on 10/17/20.
//

#include <iostream>
#include "UiLabel.h"
#include "GlobalFont.h"

UiLabel::UiLabel(double xRatio, double yRatio, double widthRatio, double heightRatio)
        : UiComponent(xRatio, yRatio, widthRatio, heightRatio) {
    text.setFillColor(sf::Color(255, 255, 255, 255));
    text.setCharacterSize(20);
    text.setFont(*GlobalFont::MainFont);
}

void UiLabel::setCharacterSize(int characterSize) {
    text.setCharacterSize(characterSize);
}

void UiLabel::resized() {
    text.setPosition(sf::Vector2f(x, y));
}

void UiLabel::mouseEnter() {
}

void UiLabel::mouseLeave() {
}

void UiLabel::draw(sf::RenderWindow *window) {
    window->draw(text);
}

void UiLabel::setFillColor(sf::Color color) {
    text.setFillColor(color);
}

void UiLabel::setText(std::string textString) {
    text.setString(textString);
}

void UiLabel::mouseHovering(int mouseX, int mouseY) {

}

void UiLabel::mouseClickedInside(int mouseX, int mouseY) {

}
