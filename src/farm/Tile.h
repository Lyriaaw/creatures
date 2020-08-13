//
// Created by Amalric Lombard de Buffières on 8/9/20.
//

#ifndef CREATURES_TILE_H
#define CREATURES_TILE_H


#include "../utils/Point.h"

class Tile {
private:
    Point position;
    float height;
    float heat;
    float ground;

public:
    Tile(const Point &position);

    float getHeight() const;

    void setHeight(float height);

    float getHeat() const;

    void setHeat(float heat);

    float getGround() const;

    void setGround(float ground);

    void addHeight(float value);
    void addHeat(float value);
    void addGround(float value);

    const Point &getPosition() const;
};


#endif //CREATURES_TILE_H
