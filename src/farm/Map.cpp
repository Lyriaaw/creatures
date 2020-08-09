//
// Created by Amalric Lombard de Buffières on 7/17/20.
//

#include "Map.h"
#include "../utils/perlin/PerlinNoise.h"

#include <iostream>
#include <zconf.h>

using namespace std;

std::mutex mapMutex;

void Map::generateRandomTerrain() {
    float seed = rand() % 1000000;
    PerlinNoise perlin(seed);

    cout << "Map generated with seed " << seed << endl;
    cout << "Map Width: " << TILE_COUNT_WIDTH << " Map Height: " << TILE_COUNT_HEIGHT << endl;

    float min = 11111110.f;
    float max = 0.f;


    for (int it = 0; it < TILE_COUNT_WIDTH; it++) {
        std::vector<Tile *> line;
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


            Tile * currentTile = new Tile();
            currentTile->setHeight(height);

            line.emplace_back(currentTile);
        }
        tiles.emplace_back(line);
    }


    float removed = ((max - min) / 2.f) + min;

    float ratio = (1.f / (max - min)) * 2.f;

    min = 11111110.f;
    max = 0.f;
    for (int it = 0; it < TILE_COUNT_WIDTH; it++) {
        for (int jt = 0; jt < TILE_COUNT_HEIGHT; jt++) {
            float currentHeight = getTileAt(it, jt)->getHeight();

            currentHeight -= removed;
            currentHeight *= ratio;
            currentHeight *= currentHeight * currentHeight;


            if (currentHeight < min) {
                min = currentHeight;
            }
            if (currentHeight > max) {
                max = currentHeight;
            }

            getTileAt(it, jt)->setHeight(currentHeight);
        }
    }
}


void Map::initRandomMap() {

    generateRandomTerrain();
}

void Map::prepareTiles() {
//    for (int it = 0; it < TILE_COUNT_WIDTH; it++) {
//        for (int jt = 0; jt < TILE_COUNT_HEIGHT; jt++) {
//
//        }
//    }
}


void Map::processGroundChanges(){

}
void Map::processClimate() {
    float newGround[TILE_COUNT_WIDTH][TILE_COUNT_HEIGHT];
    float newHeats[TILE_COUNT_WIDTH][TILE_COUNT_HEIGHT];

    for (int it = 0; it < TILE_COUNT_WIDTH; it++) {
        for (int jt = 0; jt < TILE_COUNT_HEIGHT; jt++) {
            newGround[it][jt] = getTileAt(it, jt)->getGround();
            newHeats[it][jt] = getTileAt(it, jt)->getHeat();

        }
    }



    for (int it = 0; it < TILE_COUNT_WIDTH; it++) {
        std::vector<double> groundsLine;
        for (int jt = 0; jt < TILE_COUNT_HEIGHT; jt++) {
            Tile * currentTile = tiles.at(it).at(jt);

            float availableGround = currentTile->getGround() / 50.f;


            float currentHeight = currentTile->getHeight();
            float availableHeat = currentTile->getHeat() / 20.f;

            if (availableGround == 0 && availableHeat == 0) {
                continue;
            }

            for (int x = -1; x <= 1; x++) {
                for (int y = -1; y <= 1; y++) {
                    if (it + x < 0 || it + x >= TILE_COUNT_WIDTH ||jt + y < 0 || jt + y >= TILE_COUNT_HEIGHT) {
                        continue;
                    }

                    // If the other tile is lower than this one, we add some heat to be transfer
                    // the lowest the target tile, the biggest is the added energy
                    double heightDifference = currentHeight - getTileAt(it + x, jt + y)->getHeight();
                    double transferedHeat = availableHeat + (heightDifference * availableHeat);

                    newHeats[it + x][jt + y] += transferedHeat;
                    newHeats[it][jt] -= transferedHeat;





                    newGround[it + x][jt + y] += availableGround;
                    newGround[it][jt] -= availableGround;
                }
            }

        }
    }

    for (int it = 0; it < TILE_COUNT_WIDTH; it++) {
        for (int jt = 0; jt < TILE_COUNT_HEIGHT; jt++) {
            tiles.at(it).at(jt)->setGround(newGround[it][jt]);
            tiles.at(it).at(jt)->setHeat(newHeats[it][jt]);

            if (getTileAt(it, jt)->getHeat() <= 0) {
                continue;
            }

            float heatToGroundRatio = 0.05f;
            float currentTileHeat = getTileAt(it, jt)->getHeat();

            getTileAt(it, jt)->addHeat(- 1 * currentTileHeat * heatToGroundRatio);
            getTileAt(it, jt)->addGround(currentTileHeat * heatToGroundRatio);

        }
    }

}









Tile * Map::getTileAt(int tileX, int tileY) {
    while (!mapMutex.try_lock()) {
        usleep(10);
    }
    Tile * tile = tiles.at(tileX).at(tileY);
    mapMutex.unlock();

    return tile;
}


Map::Map() {
    prepareTiles();
}

void Map::setTiles(const vector<std::vector<Tile *>> &tiles) {
    Map::tiles = tiles;
}

