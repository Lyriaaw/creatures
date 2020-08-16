//
// Created by Amalric Lombard de Buffières on 8/9/20.
//

#ifndef CREATURES_TILE_H
#define CREATURES_TILE_H


#include "../utils/Point.h"
#include <shared_mutex>

class Tile {
private:
    Point position;
    double height;
    double heat;
    double ground;

    double addedHeat;
    double addedGround;

    std::mutex add_heat_mutex;
    std::mutex add_ground_mutex;



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

    const Point &getPosition() const;

    void processAddedGround();

    void processAddedHeat();

    double getAddedHeat() const;

    double getAddedGround() const;

    double removeGround(double ground);
};


#endif //CREATURES_TILE_H
