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
#include "CreatureUI.h"
#include "../farm/Food.h"
#include "../farm/Farm.h"

class FarmUI {
private:
    Farm *farm;

    std::vector<std::vector<sf::RectangleShape>> tiles;
    std::vector<EntityUI *> entities;
public:

public:
    FarmUI(Farm *farm);
    void draw(sf::RenderWindow *window, Camera *camera, BrainConnector * selectedEntity);
    void loadMap();
    void update();
    void setPositions(Camera *camera, Creature * selectedEntity);

    void setEntities(const std::vector<EntityUI *> entities);

    void addCreature(CreatureUI * creatureUI);

    void clearEntities(std::vector<Entity *> toDelete);
    void addEntities(std::vector<Food *> to_add);
    void addCreatures(std::vector<Creature *> to_add);

    const std::vector<EntityUI *> &getEntities() const;
};


#endif //CREATURES_FARMUI_H
