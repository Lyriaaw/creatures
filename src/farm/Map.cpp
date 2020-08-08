//
// Created by Amalric Lombard de Buffières on 7/17/20.
//

#include "Map.h"
#include "../utils/perlin/PerlinNoise.h"

#include <iostream>

using namespace std;

void Map::generateRandomTerrain() {
    float seed = rand() % 1000000;
    PerlinNoise perlin(seed);

    cout << "Map generated with seed " << seed << endl;
    cout << "Map Width: " << TILE_COUNT_WIDTH << " Map Height: " << TILE_COUNT_HEIGHT << endl;

    float min = 11111110.f;
    float max = 0.f;

    for (int it = 0; it < TILE_COUNT_WIDTH; it++) {
        for (int jt = 0; jt < TILE_COUNT_HEIGHT; jt++) {

            float xComponent = (float(it) / float(TILE_COUNT_WIDTH)) * 2.5;
            float yComponent = (float(jt) / float(TILE_COUNT_HEIGHT)) * 2.5;

            float height = perlin.noise(xComponent, yComponent, 0.8);


            if (height < min) {
                min = height;
            }
            if (height > max) {
                max = height;
            }



            setTileAt(it, jt, height);
        }
    }
//    std::cout << "Min: " << min << " Max: " << max << " Removed:" << 0 << std::endl;



    float removed = ((max - min) / 2.f) + min;

    float ratio = (1.f / (max - min)) * 2.f;

    min = 11111110.f;
    max = 0.f;
    for (int it = 0; it < TILE_COUNT_WIDTH; it++) {
        for (int jt = 0; jt < TILE_COUNT_HEIGHT; jt++) {
            float currentHeight = getTileAt(it, jt);

            currentHeight -= removed;
            currentHeight *= ratio;
            currentHeight *= currentHeight * currentHeight;


            if (currentHeight < min) {
                min = currentHeight;
            }
            if (currentHeight > max) {
                max = currentHeight;
            }



            setTileAt(it, jt,  currentHeight);
        }
    }
}


void Map::initRandomMap() {
    generateRandomTerrain();
}






float Map::getTileAt(int chunkX, int chunkY) {
    return tiles[chunkX][chunkY];
}


float Map::setTileAt(int chunkX, int chunkY, float value) {
    tiles[chunkX][chunkY] = value;
}



float Map::getHeatAt(int chunkX, int chunkY) {
    return heat[chunkX][chunkY];
}


float Map::setHeatAt(int chunkX, int chunkY, float value) {
    heat[chunkX][chunkY] = value;
}

float Map::addHeatAt(int chunkX, int chunkY, float value) {
    heat[chunkX][chunkY] += value;
}



float Map::getGroundAt(int chunkX, int chunkY) {
    return ground[chunkX][chunkY];
}


float Map::setGroundAt(int chunkX, int chunkY, float value) {
    ground[chunkX][chunkY] = value;
}

float Map::addGroundAt(int chunkX, int chunkY, float value) {
    ground[chunkX][chunkY] += value;
}

