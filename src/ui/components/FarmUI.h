//
// Created by Amalric Lombard de Buffières on 7/17/20.
//

#ifndef CREATURES_FARMUI_H
#define CREATURES_FARMUI_H


#include <SFML/Graphics.hpp>
#include "../../World.h"
#include "../../farm/Map.h"
#include "Camera.h"
#include "EntityUI.h"
#include "../../farm/entities/Creature.h"
#include "CreatureUI.h"
#include "../../farm/entities/Food.h"
#include "../../farm/Farm.h"

class FarmUI {
private:
    Farm *farm;

    sf::VertexArray tilesVertexArray;
    std::vector<EntityUI *> entities;

    Point hoveredTile;
    sf::Text hoveredTileInfos;

    sf::Font * font;
public:

public:
    FarmUI(Farm *farm, sf::Font * font);
    void loadTexts();
    void draw(sf::RenderWindow *window, Camera *camera, Life * selectedEntity);
    void loadMap();
    void update();
    void setPositions(Camera *camera);
    sf::Color getColorForTile(int x, int y, Camera * camera, Map *map);

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
