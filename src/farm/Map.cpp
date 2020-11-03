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
#import <thread>
#include "../websockets/WebUiConnection.h"


using namespace std;

std::mutex mapMutex;

Map::Map(): tick(0), speedCorrectionRatio(1.0) {}

Map::Map(FarmControl * farmControl): farmControl(farmControl) {
    mapGeneratorControl = new MapGeneratorControl();
    float seed = rand() % 1000000;
    mapGeneratorControl->setSeed(seed);
}

void Map::generateRandomTerrain(bool renew) {
    PerlinNoise perlin(mapGeneratorControl->getSeed());

    cout << "Map generated with seed " << mapGeneratorControl->getSeed() << endl;
    cout << "Map Width: " << TILE_COUNT_WIDTH << " Map Height: " << TILE_COUNT_HEIGHT << endl;

    float min = 11111110.f;
    float max = 0.f;

    for (int it = 0; it < TILE_COUNT_WIDTH; it++) {
        std::vector<Tile *> line;
        for (int jt = 0; jt < TILE_COUNT_HEIGHT; jt++) {

            float xComponent = (float(it) / float(TILE_COUNT_WIDTH)) * mapGeneratorControl->getPositionRatio();
            float yComponent = (float(jt) / float(TILE_COUNT_HEIGHT)) * mapGeneratorControl->getPositionRatio();

            float height = perlin.noise(xComponent, yComponent, mapGeneratorControl->getZOrdinate());

            for (int kt = 1; kt < mapGeneratorControl->getLayers(); kt++) {
                xComponent = ((float(it) / float(TILE_COUNT_WIDTH)) + (kt * 100.0)) * (mapGeneratorControl->getPositionRatio() * kt);
                yComponent = ((float(jt) / float(TILE_COUNT_HEIGHT)) + (kt * 100.0)) * (mapGeneratorControl->getPositionRatio() * kt);


                height += perlin.noise(xComponent, yComponent, mapGeneratorControl->getZOrdinate() / (2.0 * kt));
            }


            if (height < min) {
                min = height;
            }
            if (height > max) {
                max = height;
            }



            Point coordinates = Point(it, jt);

            if (renew) {
                tiles.at(it).at(jt)->setHeight(height);
            } else {
                Tile * currentTile = new Tile(coordinates, height, 0.0, 1500.0);
                line.emplace_back(currentTile);
            }
        }
        if (!renew) {
            tiles.emplace_back(line);
        }
    }

//    std::cout << "Min: " << min << " Max: " << max << std::endl;

    float removed = ((max - min) / 2.f) + min;
    float ratio = (1.f / (max - min));



    min = 11111110.f;
    max = 0.f;


    for (int it = 0; it < TILE_COUNT_WIDTH; it++) {
        for (int jt = 0; jt < TILE_COUNT_HEIGHT; jt++) {
            float currentHeight = getTileAt(it, jt)->getHeight();

            currentHeight -= removed;
            currentHeight *= ratio;
            currentHeight += 0.5;


            if (currentHeight < min) {
                min = currentHeight;
            }
            if (currentHeight > max) {
                max = currentHeight;
            }

            getTileAt(it, jt)->setHeight(currentHeight);

        }
    }

    std::cout << "Min: " << min << " Max: " << max << std::endl;

}

void Map::initRandomMap() {

    generateRandomTerrain(false);
}

void Map::handleThread() {
    auto f = [&](){
        while (farmControl->isRunning()) {

            if (farmControl->isPaused()) {
                usleep(100000);
                continue;
            }

            processClimate();

            usleep(std::max(int(10000 + std::max(-100000, int((10000 * speedCorrectionRatio)))), 0));
        }
    };

    std::thread vegetalisationThread(f);
    vegetalisationThread.detach();
}

void Map::removeEnergyFromGround(double energyToRemove) {

    double totalGround = 0.0;

    for (int it = 0; it < TILE_COUNT_WIDTH; it++) {
        for (int jt = 0; jt < TILE_COUNT_HEIGHT; jt++) {
            Tile * currentTile = getTileAt(it, jt);
            currentTile->lockGround();
            totalGround += currentTile->getGround();
        }
    }


    double removedEnergy = 0.0;
    for (int it = 0; it < TILE_COUNT_WIDTH; it++) {
        for (int jt = 0; jt < TILE_COUNT_HEIGHT; jt++) {
            Tile * tile = getTileAt(it, jt);
            double ratio = tile->getGround() / totalGround;
            tile->lockOwnerAddGround(- 1.0 * (ratio * energyToRemove));
            removedEnergy += ratio * energyToRemove;
            tile->unlockGround();
        }
    }


}


void Map::processClimate() {
    std::chrono::system_clock::time_point start = std::chrono::system_clock::now();

    if (tick % VEGETALISATION_RATE != 0) {
        std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_time = end - start;
        dataAnalyser.getVegetalisationTime()->addValue(elapsed_time.count());
        tick++;

        triggerUpdate();

        return;
    }


    float newGround[TILE_COUNT_WIDTH][TILE_COUNT_HEIGHT];
    float newHeats[TILE_COUNT_WIDTH][TILE_COUNT_HEIGHT];

    for (int it = 0; it < TILE_COUNT_WIDTH; it++) {
        for (int jt = 0; jt < TILE_COUNT_HEIGHT; jt++) {
            Tile * currentTile = getTileAt(it, jt);
            currentTile->decayPheromone();

            currentTile->lockHeatAndGround();
            currentTile->handleEntityDecay();
            currentTile->lockOwnerAddHeat(currentTile->getAndClearTmpHeat());
            currentTile->lockOwnerAddGround(currentTile->getAndClearTmpGround());

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

            currentTile->removeDeletedEntities();


        }
    }

    vegetalisation();



    std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_time = end - start;
    dataAnalyser.getVegetalisationTime()->addValue(elapsed_time.count());
    tick++;

    triggerUpdate();

}

void Map::vegetalisation() {
    random_device rd;
    mt19937 mt(rd());
    uniform_real_distribution<double> distWidth(0, TILE_SIZE - 1 );
    uniform_real_distribution<double> distHeight(0, TILE_SIZE - 1 );

    std::vector<Entity *> addedEntities;
    for (int it = 0; it < TILE_COUNT_WIDTH; it++) {
        for (int jt = 0; jt < TILE_COUNT_HEIGHT; jt++) {
            Tile * currentTile = getTileAt(it, jt);

            float tileX = it * TILE_SIZE;
            float tileY = jt * TILE_SIZE;


            currentTile->lockGround();
            float tileAvailableEnergy = currentTile->getGround();


            int foodToGenerate = int(tileAvailableEnergy / 1000.f);
            float totalEnergyAdded = 0.f;

//            if (tileAvailableEnergy > 1950) {
//                std::cout << "Energy: " << tileAvailableEnergy << " Generate: " << foodToGenerate << std::endl;
//            }

            for (int kt = 0; kt < foodToGenerate; kt++) {
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
                float foodSize = 1;

                Entity * entity = new Entity(point);
                entity->setMass(foodSize * MASS_TO_SIZE_RATIO);
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



void Map::setTiles(const vector<std::vector<Tile *>> &tiles) {
    Map::tiles = tiles;
}

void Map::setRecordAddedEntitiesToFarm(const function<void(std::vector<Entity *>)> &recordAddedEntitiesToFarm) {
    Map::recordAddedEntitiesToFarm = recordAddedEntitiesToFarm;
}

int Map::getTick() const {
    return tick;
}

void Map::setTick(int tick) {
    Map::tick = tick;
}

void Map::setSpeedCorrectionRatio(double speedCorrectionRatio) {
    Map::speedCorrectionRatio = speedCorrectionRatio;
}

const DataAnalyser &Map::getDataAnalyser() const {
    return dataAnalyser;
}





json Map::asJson() {
    json runner;
    runner["tick"] = tick;

    json times;
    times["vegetalisation"] = this->dataAnalyser.getVegetalisationTime()->getAveragedLastValue();

    runner["times"] = times;

    return runner;
}

void Map::setTriggerUpdate(const function<void()> &triggerUpdate) {
    Map::triggerUpdate = triggerUpdate;
}

MapGeneratorControl *Map::getMapGeneratorControl() const {
    return mapGeneratorControl;
}
