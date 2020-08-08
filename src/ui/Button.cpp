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

void Button::load(std::string text, sf::Font * font) {
    background = sf::RectangleShape(sf::Vector2f(width, height));
    background.setPosition(x, y);
    background.setFillColor(backgroundColor);

    label.setFont(*font);
    label.setString(text);
    label.setCharacterSize(18);
    label.setFillColor(textColor);

    float labelWidth = label.getLocalBounds().width;
    float labelHeight = label.getLocalBounds().height;

    float labelX = x + ((width - labelWidth) / 2.f);
    float labelY = y + ((height - labelHeight) / 2.f - 5);

    label.setPosition(labelX, labelY);
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

void Button::draw(sf::RenderWindow *window) {
    window->draw(background);
    window->draw(label);
}

int Button::getId() const {
    return id;
}
