//
// Created by Amalric Lombard de Buffières on 7/17/20.
//

#ifndef CREATURES_MAP_H
#define CREATURES_MAP_H


#include <vector>
#include "../World.h"
#include "Tile.h"
#include "FarmControl.h"
#include <shared_mutex>
#include <iostream>


class Map {
private:
    FarmControl * farmControl;

    std::vector<std::vector<Tile *>> tiles;

    double speedCorrectionRatio;
    int tick;

    std::function<void(std::vector<Entity *> entities)> recordAddedEntitiesToFarm;

public:



    Map(FarmControl * farmControl);

    void initRandomMap();
    void generateRandomTerrain();
    void processClimate();
    void vegetalisation();
    void prepareTiles();

    void removeEnergyFromGround(double amount);

    Tile * getTileAt(int tileX, int tileY);

    void setTiles(const std::vector<std::vector<Tile *>> &tiles);


    void setRecordAddedEntitiesToFarm(const std::function<void(std::vector<Entity *>)> &recordAddedEntitiesToFarm);

    int getTick() const;
    void setTick(int tick);

    void setSpeedCorrectionRatio(double speedCorrectionRatio);
};


#endif //CREATURES_MAP_H
