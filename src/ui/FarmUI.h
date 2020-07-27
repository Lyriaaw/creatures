//
// Created by Amalric Lombard de Buffières on 7/17/20.
//

#ifndef CREATURES_FARMUI_H
#define CREATURES_FARMUI_H


#include <SFML/Graphics.hpp>
#include "../World.h"
#include "../farm/Map.h"

class FarmUI {
private:
    std::vector<std::vector<sf::RectangleShape>> tiles;
public:

public:
    FarmUI();
    void draw(sf::RenderWindow *window);
    void loadMap(Map map);

};


#endif //CREATURES_FARMUI_H
