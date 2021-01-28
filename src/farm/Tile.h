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

    float height;


    float pheromoneColor;
    float pheromoneQuantity;
    std::mutex pheromone_mutex;


    std::mutex entities_mutex;
    std::vector<Entity *> entities;

    std::mutex lifes_mutex;
    std::vector<Entity *> lifes;



    float heat;
    float ground;
    std::mutex ground_mutex;
    std::mutex heat_mutex;



    float tmp_heat;
    float tmp_ground;
    std::mutex tmp_ground_mutex;
    std::mutex tmp_heat_mutex;

public:

    Tile(Point coordinates, float height, float heat, float ground);

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

    void addTmpHeat(float value);

    float getTmpHeat() const;

    void setTmoHeat(float heat);

    float getAndClearTmpHeat();

    void unlockEntities();

    void lockEntities();

    std::vector<Entity *> lockOwnerGetEntities();

    void lockTmpHeatAndTmpGround();

    void unlockTmpHeatAndTmpGround();

    void addTmpGround(float value);

    float getTmpGround() const;

    float getAndClearTmpGround();

    void lockHeat();

    void unlockHeat();

    void unlockLifes();

    void lockLifes();

    void addLife(Entity *life);

    void removeLife(Entity *life);

    std::vector<Entity *> getLifesEntities();
};


#endif //CREATURES_TILE_H
