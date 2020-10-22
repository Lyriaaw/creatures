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
#include "../minimaps/Minimap.h"

class FarmUI {
private:
    Farm *farm;

    std::vector<LifeUI *> lifeUIs;
    std::vector<EntityUI *> entityUIs;

    sf::VertexArray tilesVertexArray;
    sf::VertexArray pheromoneTilesVertexArray;

    Point hoveredTile;
    sf::Text hoveredTileInfos;

    Life * selectedLife;
    Entity * selectedEntity;

    std::function<void()> selectedLifeChanged;

    Minimap * currentMinimap;
    Minimap * pheromoneMinimap;

public:

public:
    FarmUI(Farm *farm);

    void loadMap();
    void loadTexts();
    void loadFarm();
    void loadLifes();
    void loadEntities();

    void draw(sf::RenderWindow *window, Camera *camera, Life * selectedEntity);
    void update();
    void setPositions(Camera *camera);
    sf::Color getColorForTile(int x, int y, Camera * camera, Map *map);


    void mouseMoved(Point worldPosition, Camera * camera);
    void generateTileInfoText();


    void addLife(Life * life);
    void addEntity(Entity * entity);
    void addLifes(std::vector<Life *> addedToFarm);
    void addEntities(std::vector<Entity *> addedToFarm);
    void clearDeletedLifes();
    void clearDeletedEntities();

    Life *getSelectedLife() const;

    Entity *getSelectedEntity() const;

    Farm *getFarm() const;

    void setSelectedLifeChanged(const std::function<void()> &selectedLifeChanged);

    void processClick(float screenX, float screenY, Camera * camera);

    void selectedCreatureChange(std::string type);

    void setCurrentMinimap(Minimap *currentMinimap);

    const std::vector<LifeUI *> &getLifeUIs() const;

    const std::vector<EntityUI *> &getEntityUIs() const;
};


#endif //CREATURES_FARMUI_H
