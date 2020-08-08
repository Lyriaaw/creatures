//
// Created by Amalric Lombard de Buffières on 7/17/20.
//

#ifndef CREATURES_MAP_H
#define CREATURES_MAP_H


#include <vector>
#include "../World.h"

class Map {
private:
    float tiles[TILE_COUNT_WIDTH][TILE_COUNT_HEIGHT];
    float heat[TILE_COUNT_WIDTH][TILE_COUNT_HEIGHT];

public:
    void initRandomMap();
    void generateRandomTerrain();


    float getTileAt(int chunkX, int chunkY);
    float setTileAt(int chunkX, int chunkY, float value);

    float getHeatAt(int chunkX, int chunkY);
    float setHeatAt(int chunkX, int chunkY, float value);

};


#endif //CREATURES_MAP_H
