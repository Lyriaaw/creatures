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

class FarmUI {
private:
    std::vector<std::vector<sf::RectangleShape>> tiles;
    std::vector<EntityUI *> entities;
public:

public:
    FarmUI();
    void draw(sf::RenderWindow *window, Camera *camera, Entity * selectedEntity);
    void loadMap(Map map);
    void setPositions(Camera *camera);

    void setEntities(const std::vector<EntityUI *> entities);

};


#endif //CREATURES_FARMUI_H
