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
#include "../../farm/entities/Food.h"
#include "../../farm/Farm.h"
#include "LifeUI.h"

class FarmUI {
private:
    Farm *farm;

    std::vector<LifeUI *> lifeUIs;
    std::vector<EntityUI *> entityUIs;

    sf::VertexArray tilesVertexArray;

    Point hoveredTile;
    sf::Text hoveredTileInfos;

    sf::Font * font;
public:

public:
    FarmUI(Farm *farm, sf::Font * font);

    void loadMap();
    void loadTexts();
    void loadFarm();
    void loadLifes();
    void loadEntities();

    void draw(sf::RenderWindow *window, Camera *camera, Life * selectedEntity);
    void update();
    void setPositions(Camera *camera, Life * selectedEntity);
    sf::Color getColorForTile(int x, int y, Camera * camera, Map *map);


    void mouseMoved(Point worldPosition, Camera * camera);
    void generateTileInfoText();


    void addLife(Life * life);
    void addEntity(Entity * entity);
    void addLifes(std::vector<Life *> addedToFarm);
    void addEntities(std::vector<Entity *> addedToFarm);
    void clearDeletedLifes(std::vector<Life *> deletedFromFarm);
    void clearDeletedEntities(std::vector<Entity *> deletedFromFarm);

};


#endif //CREATURES_FARMUI_H
