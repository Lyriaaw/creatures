//
// Created by Amalric Lombard de Buffières on 10/17/20.
//

#include "UiButton.h"

#include <utility>
#include "GlobalFont.h"

UiButton::UiButton(int id, const std::string& textString, double xRatio, double yRatio, double widthRatio, double heightRatio)
        : UiComponent(xRatio, yRatio, widthRatio, heightRatio), id(id) {
    background.setFillColor(sf::Color(50, 50, 50, 255));

    text.setFillColor(sf::Color(255, 255, 255, 255));
    text.setCharacterSize(20);
    text.setFont(*GlobalFont::MainFont);
    text.setString(textString);

    onClick = [](int id) {
    };
}


void UiButton::resized() {
    background.setSize(sf::Vector2f(width, height));
    background.setPosition(sf::Vector2f(x, y));

    float labelWidth = text.getLocalBounds().width;
    float labelHeight = text.getLocalBounds().height;

    float labelX = x + ((width - labelWidth) / 2.f);
    float labelY = y + ((height - labelHeight) / 2.f - 5);

    text.setPosition(labelX, labelY);
}


void UiButton::mouseHovering(int mouseX, int mouseY) {

}

void UiButton::mouseClickedInside(int mouseX, int mouseY) {
    onClick(id);
}

void UiButton::mouseEnter() {
    background.setFillColor(sf::Color(75, 75, 75, 255));
}

void UiButton::mouseLeave() {
    background.setFillColor(sf::Color(50    , 50, 50, 255));
}

void UiButton::draw(sf::RenderWindow *window) {
    window->draw(background);
    window->draw(text);
}
void UiButton::setOnClick(std::function<void(int)> function) {
    onClick = std::move(function);
}