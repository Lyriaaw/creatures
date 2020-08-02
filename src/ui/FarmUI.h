//
// Created by Amalric Lombard de Buffières on 7/17/20.
//

#ifndef CREATURES_FARMUI_H
#define CREATURES_FARMUI_H


#include <SFML/Graphics.hpp>
#include "../World.h"
#include "../farm/Map.h"
#include "Camera.h"
#include "EntityUI.h"
#include "../farm/Creature.h"
#include "../farm/brain/BrainConnector.h"

class FarmUI {
private:
    Map * map;

    std::vector<std::vector<sf::RectangleShape>> tiles;
    std::vector<EntityUI *> entities;
public:

public:
    FarmUI();
    void draw(sf::RenderWindow *window, Camera *camera, BrainConnector * selectedEntity);
    void loadMap(Map *map);
    void setPositions(Camera *camera, Creature * selectedEntity);

    void setEntities(const std::vector<EntityUI *> entities);

};


#endif //CREATURES_FARMUI_H
