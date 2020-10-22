//
// Created by Amalric Lombard de Buffières on 7/17/20.
//

#include "Map.h"
#include "../utils/perlin/PerlinNoise.h"
#include "../utils/Point.h"
#include "entities/Food.h"

#include <iostream>
#include <zconf.h>
#include <random>

using namespace std;

std::mutex mapMutex;

void Map::generateRandomTerrain() {
    float seed = rand() % 1000000;
    PerlinNoise perlin(seed);
    PerlinNoise colorPerlin(seed * 3.14);

    cout << "Map generated with seed " << seed << endl;
    cout << "Map Width: " << TILE_COUNT_WIDTH << " Map Height: " << TILE_COUNT_HEIGHT << endl;

    float min = 11111110.f;
    float max = 0.f;

    float minColor = 11111110.f;
    float maxColor = 0.f;




    for (int it = 0; it < TILE_COUNT_WIDTH; it++) {
        std::vector<Tile *> line;
        for (int jt = 0; jt < TILE_COUNT_HEIGHT; jt++) {

            float xComponent = (float(it) / float(TILE_COUNT_WIDTH)) * 2.5;
            float yComponent = (float(jt) / float(TILE_COUNT_HEIGHT)) * 2.5;

            float height = perlin.noise(xComponent, yComponent, 0.8);
            float color = colorPerlin.noise(xComponent, yComponent, 0.8);


            if (height < min) {
                min = height;
            }
            if (height > max) {
                max = height;
            }

            if (color < minColor) {
                minColor = color;
            }
            if (color > maxColor) {
                maxColor = color;
            }



            Point coordinates = Point(it, jt);

            Tile * currentTile = new Tile(coordinates, height, color, 0.0, 1500.0);

            line.emplace_back(currentTile);
        }
        tiles.emplace_back(line);
    }


    float removed = ((max - min) / 2.f) + min;
    float ratio = (1.f / (max - min)) * 2.f;

    float removedColor = ((maxColor - minColor) / 2.f) + minColor;
    float ratioColor = (1.f / (maxColor - minColor)) * 2.f;



    min = 11111110.f;
    max = 0.f;

    minColor = 11111110.f;
    maxColor = 0.f;


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


            float currentColor = getTileAt(it, jt)->getColor();

            currentColor -= removedColor;
            currentColor *= ratioColor;


            if (currentColor < minColor) {
                minColor = currentColor;
            }
            if (currentColor > maxColor) {
                maxColor = currentColor;
            }

            getTileAt(it, jt)->setColor(abs(currentColor));



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

void Map::removeEnergyFromGround(double energyToRemove) {

    double totalGround = 0.0;

    for (int it = 0; it < TILE_COUNT_WIDTH; it++) {
        for (int jt = 0; jt < TILE_COUNT_HEIGHT; jt++) {
            Tile * currentTile = getTileAt(it, jt);
            totalGround += currentTile->getGround();
        }
    }


    double removedEnergy = 0.0;
    for (int it = 0; it < TILE_COUNT_WIDTH; it++) {
        for (int jt = 0; jt < TILE_COUNT_HEIGHT; jt++) {
            Tile * tile = getTileAt(it, jt);
            double ratio = tile->getGround() / totalGround;
            tile->removeGround(ratio * energyToRemove);
            removedEnergy += ratio * energyToRemove;
        }
    }


}


void Map::processClimate() {
    float newGround[TILE_COUNT_WIDTH][TILE_COUNT_HEIGHT];
    float newHeats[TILE_COUNT_WIDTH][TILE_COUNT_HEIGHT];

    for (int it = 0; it < TILE_COUNT_WIDTH; it++) {
        for (int jt = 0; jt < TILE_COUNT_HEIGHT; jt++) {
            Tile * currentTile = getTileAt(it, jt);
            currentTile->handleEntityDecay();
            currentTile->decayPheromone();
            currentTile->removeDeletedEntities();

            currentTile->lockHeatAndGround();
            newGround[it][jt] = currentTile->getGround();
            newHeats[it][jt] = currentTile->getHeat();

        }
    }



    for (int it = 0; it < TILE_COUNT_WIDTH; it++) {
        for (int jt = 0; jt < TILE_COUNT_HEIGHT; jt++) {
            Tile * currentTile = tiles.at(it).at(jt);

            float availableGround = currentTile->getGround() / 1000.f;


            float currentHeight = currentTile->getHeight();
            float availableHeat = currentTile->getHeat() / 10;

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
                    double heightDifference = (currentHeight - getTileAt(it + x, jt + y)->getHeight());

                    double transferedHeat = availableHeat + (heightDifference * availableHeat);
                    newHeats[it + x][jt + y] += transferedHeat;
                    newHeats[it][jt] -= transferedHeat;




                    double transferedGround = availableGround + (heightDifference * availableGround);

                    newGround[it + x][jt + y] += transferedGround;
                    newGround[it][jt] -= transferedGround;
                }
            }

        }
    }





    for (int it = 0; it < TILE_COUNT_WIDTH; it++) {
        for (int jt = 0; jt < TILE_COUNT_HEIGHT; jt++) {
            Tile * currentTile = getTileAt(it, jt);
            currentTile->lockOwnerSetGround(newGround[it][jt]);
            currentTile->lockOwnerSetHeat(newHeats[it][jt]);

            if (currentTile->getHeat() <= 0) {
                currentTile->unlockHeatAndGround();

                continue;
            }

            float heatToGroundRatio = 0.01f * VEGETALISATION_RATE;
            float currentTileHeat = currentTile->getHeat();

            currentTile->lockOwnerAddHeat(- 1 * currentTileHeat * heatToGroundRatio);
            currentTile->lockOwnerAddGround(currentTileHeat * heatToGroundRatio);

            currentTile->unlockHeatAndGround();

        }
    }

    vegetalisation();
}

void Map::vegetalisation() {
    random_device rd;
    mt19937 mt(rd());
    uniform_real_distribution<double> distWidth(-10, TILE_SIZE + 10);
    uniform_real_distribution<double> distHeight(-10, TILE_SIZE + 10);

    std::vector<Entity *> addedEntities;
    for (int it = 0; it < TILE_COUNT_WIDTH; it++) {
        for (int jt = 0; jt < TILE_COUNT_HEIGHT; jt++) {
            Tile * currentTile = getTileAt(it, jt);

            float tileX = it * TILE_SIZE;
            float tileY = jt * TILE_SIZE;


            currentTile->lockGround();
            float tileAvailableEnergy = currentTile->getGround();


            int foodToGenerate = int(tileAvailableEnergy / 2000.f);
            float totalEnergyAdded = 0.f;

//            if (tileAvailableEnergy > 1950) {
//                std::cout << "Energy: " << tileAvailableEnergy << " Generate: " << foodToGenerate << std::endl;
//            }

            for (int it = 0; it < foodToGenerate; it++) {
                int x = distWidth(mt);
                int y = distHeight(mt);

                Point point(tileX + x, tileY + y);

                if (point.getX() >= FARM_WIDTH) {
                    point.setX(FARM_WIDTH - 1);
                }
                if (point.getY() >= FARM_HEIGHT) {
                    point.setY(FARM_HEIGHT - 1);
                }
                if (point.getX() < 0) {
                    point.setX(0);
                }
                if (point.getY() < 0) {
                    point.setY(0);
                }



                //        float foodSize = ((rand() % 300) / 100.f) + 2;
                float foodSize = 2;

                Entity * entity = new Entity(point);
                entity->setMass(2 * MASS_TO_SIZE_RATIO);
                entity->setColor(0.28f);
                entity->setBrightness(0.3f);

                totalEnergyAdded += entity->getMass();

                Point tilePosition = entity->getTileCoordinates();
                getTileAt(tilePosition.getX(), tilePosition.getY())->addEntity(entity);
                addedEntities.emplace_back(entity);
            }

            currentTile->lockOwnerAddGround(-1 * totalEnergyAdded);
            currentTile->unlockGround();
        }
    }


    recordAddedEntitiesToFarm(addedEntities);
}











Tile * Map::getTileAt(int tileX, int tileY) {
    while (!mapMutex.try_lock()) {
        usleep(10);
    }

    if (tileX < 0 || tileX >= TILE_COUNT_WIDTH || tileY < 0 || tileY >= TILE_COUNT_HEIGHT) {
        std::cout << "ERROR, REQUESTED WRONG TILE => X: " << tileX << " Y: " << tileY << std::endl;
        return tiles.at(0).at(0);
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

void Map::setRecordAddedEntitiesToFarm(const function<void(std::vector<Entity *>)> &recordAddedEntitiesToFarm) {
    Map::recordAddedEntitiesToFarm = recordAddedEntitiesToFarm;
}

