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

    sf::VertexArray tilesVertexArray;
    std::vector<EntityUI *> entities;

    Point hoveredTile;
    sf::Text hoveredTileInfos;
public:

public:
    FarmUI(Farm *farm, sf::Font * font);
    void loadTexts(sf::Font * font);
    void draw(sf::RenderWindow *window, Camera *camera, BrainConnector * selectedEntity);
    void loadMap();
    void update();
    void setPositions(Camera *camera);

    void setEntities(const std::vector<EntityUI *> entities);

    void mouseMoved(Point worldPosition, Camera * camera);
    void generateTileInfoText();

    void addCreature(CreatureUI * creatureUI);

    void clearEntities(std::vector<Entity *> toDelete);
    void addEntities(std::vector<Food *> to_add);
    void addCreatures(std::vector<Creature *> to_add);

    const std::vector<EntityUI *> &getEntities() const;
};


#endif //CREATURES_FARMUI_H
