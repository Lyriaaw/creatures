//
// Created by Amalric Lombard de Buffières on 8/8/20.
//

#include "Button.h"


Button::Button(std::string text, int id, sf::Font * font, int x, int y, int width, int height,
                const sf::Color &backgroundColor,
                const sf::Color &textColor) : background(background),
                                              backgroundColor(backgroundColor),
                                              textColor(textColor), x(x), y(y),
                                              width(width), height(height), id(id) {
    load(text, font);
}

void Button::placeOnScreen() {
    background.setPosition(x, y);

    float labelWidth = label.getLocalBounds().width;
    float labelHeight = label.getLocalBounds().height;

    float labelX = x + ((width - labelWidth) / 2.f);
    float labelY = y + ((height - labelHeight) / 2.f - 5);

    label.setPosition(labelX, labelY);
}

void Button::move(int x, int y) {
    this->x = x;
    this->y = y;

    placeOnScreen();
}

void Button::load(std::string text, sf::Font * font) {
    background = sf::RectangleShape(sf::Vector2f(width, height));
    background.setFillColor(backgroundColor);

    label.setFont(*font);
    label.setString(text);
    label.setCharacterSize(18);
    label.setFillColor(textColor);

    placeOnScreen();
}

bool Button::clicked(int mouseX, int mouseY) {
    if (x > mouseX || y > mouseY) {
        return false;
    }

    if (x + width < mouseX || y + height < mouseY) {
        return false;
    }


    return true;
}

void Button::changeBackgroundColor(sf::Color color){
    if (color == backgroundColor) {
        return;
    }

    backgroundColor = color;
    background.setFillColor(backgroundColor);
}

void Button::draw(sf::RenderWindow *window) {
    window->draw(background);
    window->draw(label);
}

int Button::getId() const {
    return id;
}

int Button::getX() const {
    return x;
}

int Button::getY() const {
    return y;
}
