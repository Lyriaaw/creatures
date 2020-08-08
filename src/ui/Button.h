//
// Created by Amalric Lombard de Buffières on 8/8/20.
//

#ifndef CREATURES_BUTTON_H
#define CREATURES_BUTTON_H


#include <SFML/Graphics.hpp>

class Button {
private:
    sf::Text label;
    sf::RectangleShape background;

    sf::Color backgroundColor;
    sf::Color textColor;

    int x, y, width, height;

    int id;
public:

    Button(std::string text, int id, sf::Font * font, int x, int y, int width, int height,
           const sf::Color &backgroundColor,
           const sf::Color &textColor);

    void load(std::string text, sf::Font * font);

    void draw(sf::RenderWindow *window);

    bool clicked(int mouseX, int mouseY);

    int getId() const;
};


#endif //CREATURES_BUTTON_H
