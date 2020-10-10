//
// Created by Amalric Lombard de Buffières on 8/9/20.
//

#ifndef CREATURES_TILE_H
#define CREATURES_TILE_H


#include "../utils/Point.h"

class Tile {
private:
    Point position;
    double height;
    double heat;
    double ground;

public:
    Tile(const Point &position);

    double getHeight() const;

    void setHeight(double height);

    double getHeat() const;

    void setHeat(double heat);

    double getGround() const;

    void setGround(double ground);

    void addHeight(double value);
    void addHeat(double value);
    void addGround(double value);

    void removeHeat(double value);
    void removeGround(double value);



    const Point &getPosition() const;
};


#endif //CREATURES_TILE_H
