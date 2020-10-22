//
// Created by Amalric Lombard de Buffières on 8/9/20.
//

#ifndef CREATURES_TILE_H
#define CREATURES_TILE_H


#include "../utils/Point.h"
#include "entities/Entity.h"

class Tile {
private:
    Point coordinates;

    float color;
    float height;
    float heat;
    float ground;

    float pheromoneColor;
    float pheromoneQuantity;

    std::mutex entities_mutex;
    std::vector<Entity *> entities;

    std::mutex ground_mutex;
    std::mutex heat_mutex;

public:

    Tile(Point coordinates, float height, float color, float heat, float ground);

    float getHeight() const;

    void setHeight(float height);

    float getHeat() const;

    void setHeat(float heat);

    float getGround() const;

    void setGround(float ground);

    void addHeight(float value);
    void addHeat(float value);
    void addGround(float value);

    void removeGround(float groundToRemove);

    float getColor() const;

    void setColor(float color);

    float getPheromoneColor() const;

    float getPheromoneQuantity() const;


    void addPheromone(float color, float quantity);
    void decayPheromone();

    void removePhermoneQuantity(float quantityRemoved);

    const Point &getCoordinates() const;


    void addEntity(Entity *entity);

    std::vector<Entity *> getEntities();

    void handleEntityDecay();

    void removeDeletedEntities();

    void lockHeatAndGround();

    void unlockHeatAndGround();

    void lockOwnerSetHeat(float heat);

    void lockOwnerSetGround(float ground);

    void lockOwnerAddGround(float value);

    void lockOwnerAddHeat(float value);

    void lockGround();

    void unlockGround();
};


#endif //CREATURES_TILE_H
