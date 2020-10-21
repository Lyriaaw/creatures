//
// Created by Amalric Lombard de Buffières on 8/9/20.
//

#ifndef CREATURES_TILE_H
#define CREATURES_TILE_H


#include "../utils/Point.h"

class Tile {
private:
    Point coordinates;

    float color;
    float height;
    float heat;
    float ground;

    float pheromoneColor;
    float pheromoneQuantity;

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
};


#endif //CREATURES_TILE_H
