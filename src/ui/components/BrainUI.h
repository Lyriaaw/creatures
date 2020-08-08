//
// Created by Amalric Lombard de Buffières on 8/2/20.
//

#ifndef CREATURES_BRAINUI_H
#define CREATURES_BRAINUI_H


#include <SFML/Graphics.hpp>
#include "../../farm/brain/Brain.h"

class BrainUI {
private:
    Brain * brain;


    sf::RectangleShape background;
    std::vector<sf::CircleShape> neurons;
    std::vector<sf::Text> neuronNames;
    sf::VertexArray links;

    float x, y, width, height;

public:
    BrainUI(Brain *brain, float x, float y, float width, float height, sf::Font * font);

    void initialize(sf::Font * font);
    void draw(sf::RenderWindow *window);
};


#endif //CREATURES_BRAINUI_H
