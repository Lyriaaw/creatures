//
// Created by Amalric Lombard de Buffières on 7/17/20.
//

#ifndef CREATURES_FARMUI_H
#define CREATURES_FARMUI_H


#include <SFML/Graphics.hpp>
#include "../farm/Farm.h"

class FarmUI {
private:
    sf::RectangleShape rectanglesMap[CHUNK_COUNT_WIDTH][CHUNK_COUNT_HEIGHT];
    Farm *farm;

public:
    FarmUI(Farm *farm);
    void draw(sf::RenderWindow *window);

};


#endif //CREATURES_FARMUI_H
