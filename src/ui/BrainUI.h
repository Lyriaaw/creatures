//
// Created by Amalric Lombard de Buffières on 8/2/20.
//

#ifndef CREATURES_BRAINUI_H
#define CREATURES_BRAINUI_H


#include <SFML/Graphics.hpp>
#include "../farm/brain/Brain.h"

class BrainUI {
private:
    Brain * brain;


    sf::RectangleShape background;
    std::vector<sf::CircleShape> neurons;

    float x, y, width, height;

public:
    BrainUI(Brain *brain, float x, float y, float width, float height);

    void initialize();
    void draw(sf::RenderWindow *window);
};


#endif //CREATURES_BRAINUI_H
