//
// Created by Amalric Lombard de Buffières on 7/17/20.
//

#ifndef CREATURES_MAP_H
#define CREATURES_MAP_H


#include <vector>
#include "../World.h"
#include "Tile.h"
#include <shared_mutex>
#include <iostream>


class Map {
private:
    std::vector<std::vector<Tile *>> tiles;


    std::function<void(std::vector<Entity *> entities)> recordAddedEntitiesToFarm;

public:



    Map();

    void initRandomMap();
    void generateRandomTerrain();
    void processClimate();
    void processGroundChanges();
    void prepareTiles();

    void removeEnergyFromGround(double amount);

    Tile * getTileAt(int tileX, int tileY);

    void setTiles(const std::vector<std::vector<Tile *>> &tiles);

    void vegetalisation();

    void setRecordAddedEntitiesToFarm(const std::function<void(std::vector<Entity *>)> &recordAddedEntitiesToFarm);
};


#endif //CREATURES_MAP_H
