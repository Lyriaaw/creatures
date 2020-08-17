//
// Created by Amalric Lombard de Buffières on 7/17/20.
//

#include "Farm.h"
#include <zconf.h>

#include <iostream>
#include <sstream>
#include <iomanip>
#include <thread>
#include "../utils/perlin/PerlinNoise.h"


using namespace std;

Farm::Farm(){
    tickStart = std::chrono::system_clock::now();
    tickEnd = std::chrono::system_clock::now();
    dataAnalyser = new DataAnalyser();
}

void Farm::generateRandomTerrain(int seed) {

    cout << "Map generated with seed " << seed << endl;
    cout << "Chunks: " << CHUNK_COUNT_WIDTH << " x " << CHUNK_COUNT_HEIGHT << endl;
    cout << "Tiles: " << TILE_COUNT_WIDTH << " x " << TILE_COUNT_HEIGHT << endl;
    cout << "Size: " << FARM_WIDTH << " x " << FARM_HEIGHT << endl;
    cout << "Tiles per chunks: " << TILE_PER_CHUNK << endl;

    float min = 11111110.f;
    float max = 0.f;
    PerlinNoise perlin(seed);
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
        }
    }

    for (int it = 0; it < CHUNK_COUNT_WIDTH; it++) {
        std::vector<Chunk *> chunkLine;
        for (int jt = 0; jt < CHUNK_COUNT_HEIGHT; jt++) {
            Chunk * chunk = new Chunk(Point(it, jt), nursery, dataAnalyser);
            chunk->generateRandomChunk(seed, min, max);

            chunkLine.emplace_back(chunk);
        }
        chunks.emplace_back(chunkLine);
    }

    for (int it = 0; it < CHUNK_COUNT_WIDTH; it++) {
        for (int jt = 0; jt < CHUNK_COUNT_HEIGHT; jt++) {

            Chunk * chunk = chunks.at(it).at(jt);

            for (int x = -1; x <= 1; x++) {
                for (int y = -1; y <= 1; y++) {
                    if (it + x < 0 || it + x >= CHUNK_COUNT_WIDTH ||jt + y < 0 || jt + y >= CHUNK_COUNT_HEIGHT) {
                        continue;
                    }

                    Chunk * neighbour = chunks.at(it + x).at(jt + y);
                    chunk->setNeighbour(x, y, neighbour);
                }
            }

        }
    }
}




void Farm::InitFromRandom() {
    float seed = rand() % 1000000;




    random_device rd;
    mt19937 mt(rd());
    uniform_real_distribution<double> distWidth(0, FARM_WIDTH);
    uniform_real_distribution<double> distHeight(0, FARM_HEIGHT);

//    map = new Map();
//    map->initRandomMap(seed);


    std::vector<std::vector<std::vector<Entity *>>> testEntites;

    for (int it = 0; it < TILE_COUNT_WIDTH; it++) {
        std::vector<std::vector<Entity *>> line;
        for (int jt = 0; jt < TILE_COUNT_HEIGHT; jt++) {
            std::vector<Entity *> currentChunk;
            line.push_back(currentChunk);
        }
        testEntites.emplace_back(line);
    }

    entityGrid = testEntites;


    for (int it = 0; it < CHUNK_COUNT_WIDTH; it++) {
        std::vector<std::vector<Entity *>> chunkEntityLine;
        std::vector<std::vector<Life *>> chunkLifeLine;
        for (int jt = 0; jt < CHUNK_COUNT_HEIGHT; jt++) {
            std::vector<Entity *> chunkEntityCase;
            std::vector<Life *> chunkLifeCase;
            chunkEntityLine.push_back(chunkEntityCase);
            chunkLifeLine.push_back(chunkLifeCase);
        }
        chunkEntityGrid.emplace_back(chunkEntityLine);
        chunkLifeGrid.emplace_back(chunkLifeLine);
    }

    std::uniform_real_distribution<float> distMovement(-1, 1);
    nursery = new CreatureNursery();
    for (int it = 0; it < INITIAL_CREATURE_COUNT; it++) {
        Life * initialLife = nursery->generateCreatureFromRandom();

        float creatureEnergy = initialLife->getEnergyManagement()->getMaxMass() / 2.0;
        initialLife->getEnergyManagement()->setEnergy(creatureEnergy);
        initialLife->setMass(creatureEnergy);

        lifes.push_back(initialLife);
    }

    for (int it = 0; it < INITIAL_FOOD_COUNT; it++) {
        Life * initialLife = nursery->generateVegetalFromRandom();

        float creatureEnergy = initialLife->getEnergyManagement()->getMaxMass() / 2.0;
        initialLife->getEnergyManagement()->setEnergy(creatureEnergy);
        initialLife->setMass(creatureEnergy);

        lifes.push_back(initialLife);
    }

    availableEnergy = 0.f;
    tickCount = 0;

    sortCreatures();


    generateRandomTerrain(seed);

    generateEntityGrid();

    for (int it = 0; it < CHUNK_COUNT_WIDTH; it++) {
        for (int jt = 0; jt < CHUNK_COUNT_HEIGHT; jt++) {

            getChunkAt(it, jt)->setLifes(chunkLifeGrid.at(it).at(jt));
            getChunkAt(it, jt)->setEntities(chunkEntityGrid.at(it).at(jt));
            getChunkAt(it, jt)->setStep("READY_TO_START");

        }
    }

}


void Farm::Tick(bool paused) {



     if (!paused) {
        populationControl();
    }




    if (!paused) {
        statistics();
    }
    sortCreatures();


    tickEnd = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_time = tickEnd - tickStart;
    tickStart = std::chrono::system_clock::now();

    if (!paused) {
        double tickTime = elapsed_time.count();
        if (tickCount == 1) {
            dataAnalyser->getTickTime()->addValue(0);
        } else {
            dataAnalyser->getTickTime()->addValue(tickTime);
        }


        dataAnalyser->getTickPerSecond()->addValue(1.0 / tickTime);
    }
}

void Farm::handleBigThread(bool *paused, bool *running) {

    std::thread chunkThreads[CHUNK_COUNT_WIDTH * CHUNK_COUNT_HEIGHT];

    for (int it = 0; it < CHUNK_COUNT_WIDTH; it++) {
        for (int jt = 0; jt < CHUNK_COUNT_HEIGHT; jt++) {

            auto f = [](Chunk * chunk, bool *running, bool *paused) {
                while(*running) {
                    chunk->generateEntityGrid();

                    if (!*paused) {
                        chunk->handleEnergyGiveaway();
                        chunk->processClimate();
                    }

                    chunk->brainProcessing();

                    if (!*paused) {
                        chunk->executeCreaturesActions();
                        chunk->moveCreatures();
                        chunk->statistics();
                        chunk->aTickHavePassed();
                    }
                }
            };

            int index = (it * CHUNK_COUNT_HEIGHT) + jt;

            chunkThreads[index] = std::thread(f, chunks.at(it).at(jt), running, paused);

        }
    }

    for (int it = 0; it < CHUNK_COUNT_WIDTH; it++) {
        for (int jt = 0; jt < CHUNK_COUNT_HEIGHT; jt++) {
            int index = (it * CHUNK_COUNT_HEIGHT) + jt;

            chunkThreads[index].join();
        }
    }

}







void Farm::populationControl() {
    std::chrono::system_clock::time_point start = std::chrono::system_clock::now();

    if (this->lifes.size() > int(INITIAL_CREATURE_COUNT / 2) - (INITIAL_CREATURE_COUNT * 0.05)) {
        std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_time = end - start;
        dataAnalyser->getPopulationControlTime()->addValue(elapsed_time.count());
        return;
    }



    random_device rd;
    mt19937 mt(rd());
    uniform_real_distribution<double> distWidth(0, FARM_WIDTH);
    uniform_real_distribution<double> distHeight(0, FARM_HEIGHT);

    std::vector<Life *> sortedConnectors = getScoreSortedCreatures();
    int selectedParentCount = sortedConnectors.size() / 2;

    int newConnectorNeeded = int(INITIAL_CREATURE_COUNT / 2) + (INITIAL_CREATURE_COUNT * 0.05) - this->lifes.size();

    float totalEnergyRemoved = 0.f;
    for (int it = 0; it < newConnectorNeeded; it++) {
        int fatherIndex = rand() % selectedParentCount;
        int motherIndex = rand() % selectedParentCount;

        Life * father = sortedConnectors.at(fatherIndex);
        Life * mother = sortedConnectors.at(motherIndex);

        Life * child = this->nursery->Mate(father, mother);
        child->getEnergyManagement()->setEnergy(child->getEnergyManagement()->getMaxMass() / 4.f);
        child->setMass(child->getEnergyManagement()->getMaxMass() / 4.f);
        Entity * childCreature = child->getEntity();

//        float childSpawnX = distWidth(mt);
//        float childSpawnY = distHeight(mt);
//        Point childCreaturePosition = Point(childSpawnX, childSpawnY);
//
//        childCreature->setPosition(childCreaturePosition);

        totalEnergyRemoved += child->getEntity()->getMass() + child->getEnergyManagement()->getEnergy();


        lifes.emplace_back(child);
        lifesAdded.emplace_back(child);
    }

    // TODO
//    map->removeEnergyFromGround(totalEnergyRemoved);



    std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_time = end - start;
    dataAnalyser->getPopulationControlTime()->addValue(elapsed_time.count());
}


void Farm::aTickHavePassed() {
    for (int it = 0; it < lifes.size(); it++) {
        lifes.at(it)->getEntity()->aTickHavePassed();
    }
    for (int it = 0; it < entities.size(); it++) {
        entities.at(it)->aTickHavePassed();
    }
    tickCount++;
}

void Farm::waitForChunkReadyForStatistics() {
    bool allNeighboursReady(true);

    std::vector<std::string> steps;

    do {
        allNeighboursReady = true;

        for (int it = 0; it < CHUNK_COUNT_WIDTH; it++) {
            for (int jt = 0; jt < CHUNK_COUNT_HEIGHT; jt++) {

                Chunk * chunk = getChunkAt(it, jt);

                bool found(false);
                for (int kt = 0; kt < steps.size(); kt++) {
                    if (chunk->getStep() == steps.at(kt)) {
                        found = true;
                    }
                }

                if (!found) {
                    allNeighboursReady = false;
                }

            }
        }

//        if (!allNeighboursReady) {
//            usleep(10);
//        }

    }while (!allNeighboursReady);
}

void Farm::statistics() {


    std::chrono::system_clock::time_point start = std::chrono::system_clock::now();

    std::vector<Life *> currentLifes = fetchLifes();

    std::vector<Life *> sortedLife = getScoreSortedCreatures();
//    std::vector<BrainConnector *> sortedConnectors = connectors;
    int populationSize = currentLifes.size();

    dataAnalyser->getPopulation()->addValue(populationSize);

    double totalPopulationScore = 0.0;
    for (int it = 0; it < populationSize; it++) {
        totalPopulationScore += currentLifes.at(it)->getEntity()->getAge();
    }

    double averagePopulationAge = totalPopulationScore / double(populationSize);

    double maxScore = currentLifes.at(0)->getEntity()->getAge();

    double firstQuartileScore = currentLifes.at(populationSize / 4)->getEntity()->getAge();
    double median = currentLifes.at(populationSize / 2)->getEntity()->getAge();
    double lastQuartileScore = currentLifes.at((3 * populationSize) / 4)->getEntity()->getAge();


    dataAnalyser->getAverageScore()->addValue(averagePopulationAge);
    dataAnalyser->getBestScore()->addValue(maxScore);
    dataAnalyser->getFirstQuartileScore()->addValue(firstQuartileScore);
    dataAnalyser->getMedianScore()->addValue(median);
    dataAnalyser->getLastQuartileScore()->addValue(lastQuartileScore);




    double totalCreaturesEnergy = 0.f;
    double totalCreaturesMass = 0.f;
    double totalFoodsMass = 0.f;


    for (int it = 0; it < currentLifes.size(); it++) {
        Life * currentLife = currentLifes.at(it);
        totalCreaturesEnergy += currentLife->getEnergyManagement()->getEnergy();
        totalCreaturesMass += currentLife->getEntity()->getMass();
    }

//    for (int it = 0; it < entities.size(); it++) {
//        Entity * entity = entities.at(it);
//        totalFoodsMass += entity->getMass();
//    }


    double totalHeat = 0.0;
    double totalGround = 0.0;
    double totalToAdd = 0.0;
    for (int it = 0; it < TILE_COUNT_WIDTH; it++) {
        for (int jt = 0; jt < TILE_COUNT_HEIGHT; jt++) {
            totalHeat += getTileAt(it, jt)->getHeat();
            totalGround += getTileAt(it, jt)->getGround();
            totalToAdd += getTileAt(it, jt)->getAddedHeat();
            totalToAdd += getTileAt(it, jt)->getAddedGround();
        }
    }


    int totalEnergy = availableEnergy + totalFoodsMass + totalCreaturesMass + totalCreaturesEnergy + totalHeat + totalGround + totalToAdd;

//    std::cout << "Tick: " << tickCount << " Total: " << totalEnergy << " Difference: " << totalEnergy - dataAnalyser->getTotalEnergy()->getLastValue() << std::endl;

    dataAnalyser->getTotalEnergy()->addValue(totalEnergy);
    dataAnalyser->getAvailableEnergy()->addValue(availableEnergy);
    dataAnalyser->getFoodEnergy()->addValue(totalFoodsMass);
    dataAnalyser->getCreaturesMass()->addValue(totalCreaturesMass);
    dataAnalyser->getCreaturesEnergy()->addValue(totalCreaturesEnergy);
    dataAnalyser->getHeatEnergy()->addValue(totalHeat);
    dataAnalyser->getGroundEnergy()->addValue(totalGround);
    dataAnalyser->getEnergyToAdd()->addValue(totalToAdd);




    double totalTime = 0.0;
    totalTime += dataAnalyser->getEntityGridTime()->getLastValue();
    totalTime += dataAnalyser->getBrainProcessingTime()->getLastValue();
    totalTime += dataAnalyser->getBrainOutputsTime()->getLastValue();
    totalTime += dataAnalyser->getPrepareActionsTime()->getLastValue();
    totalTime += dataAnalyser->getExecuteActionsTime()->getLastValue();
    totalTime += dataAnalyser->getMoveCreaturesTime()->getLastValue();
    totalTime += dataAnalyser->getPopulationControlTime()->getLastValue();
    totalTime += dataAnalyser->getVegetalisationTime()->getLastValue();

    std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_time = end - start;
    double statisticsTime = elapsed_time.count();

    dataAnalyser->getStatisticsTime()->addValue(statisticsTime);
    totalTime += statisticsTime;

    dataAnalyser->getTotalTime()->addValue(totalTime);




    for (int it = 0; it < CHUNK_COUNT_WIDTH; it++) {
        for (int jt = 0; jt < CHUNK_COUNT_HEIGHT; jt++) {

            Chunk * chunk = getChunkAt(it, jt);
            chunk->setStep("READY_TO_START");
        }
    }
}

void Farm::generateEntityGrid() {
    std::chrono::system_clock::time_point start = std::chrono::system_clock::now();

    for (int it = 0; it < TILE_COUNT_WIDTH; it++) {
        for (int jt = 0; jt < TILE_COUNT_HEIGHT; jt++) {
            entityGrid.at(it).at(jt).clear();
        }
    }


    for (int it = 0; it < CHUNK_COUNT_WIDTH; it++) {
        for (int jt = 0; jt < CHUNK_COUNT_HEIGHT; jt++) {
            chunkLifeGrid.at(it).at(jt).clear();
            chunkEntityGrid.at(it).at(jt).clear();
        }
    }



    for (int it = 0; it < lifes.size(); it++) {
        Point position = lifes.at(it)->getEntity()->getPosition();
        Point tileCoordinate = position.getTileCoordinates();
        Point chunkCoordinates = position.getSimpleCoordinates();

        entityGrid.at(tileCoordinate.getX()).at(tileCoordinate.getY()).push_back(lifes.at(it)->getEntity());
        chunkLifeGrid.at(chunkCoordinates.getX()).at(chunkCoordinates.getY()).push_back(lifes.at(it));
    }

    for (int it = 0; it < entities.size(); it++) {
        Point position = entities.at(it)->getPosition();
        Point tileCoordinate = position.getTileCoordinates();
        Point chunkCoordinates = position.getSimpleCoordinates();

        entityGrid.at(tileCoordinate.getX()).at(tileCoordinate.getY()).push_back(entities.at(it));
        chunkEntityGrid.at(chunkCoordinates.getX()).at(chunkCoordinates.getY()).push_back(entities.at(it));

    }



    std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_time = end - start;
    dataAnalyser->getEntityGridTime()->addValue(elapsed_time.count());
}



std::vector<Life *> Farm::getScoreSortedCreatures() {
    return sorted;
}

void Farm::sortCreatures() {
    std::vector<Life *> sortResult;

    std::vector<Life *> tmpLifes = this->lifes;

    while (!tmpLifes.empty()) {

        int biggestIndex = -1;
        float biggestScore = 0.f;

        for (int it = 0; it < tmpLifes.size(); it++) {
            if (tmpLifes.at(it)->getEntity()->getAge() >= biggestScore) {
                biggestIndex = it;
                biggestScore = tmpLifes.at(it)->getEntity()->getAge();
            }
        }

        if (biggestIndex == -1) {
            std::cout << "Error while sorting creatures by score - Life will probably crash" << std::endl;
        }

        sortResult.emplace_back(tmpLifes.at(biggestIndex));
        tmpLifes.erase(tmpLifes.begin() + biggestIndex);
    }
    this->sorted = sortResult;
}












CreatureNursery *Farm::getNursery() const {
    return nursery;
}


void Farm::setDataAnalyser(DataAnalyser *dataAnalyser) {
    Farm::dataAnalyser = dataAnalyser;
}

const vector<std::vector<std::vector<Entity *>>> &Farm::getEntityGrid() const {
    return entityGrid;
}

const vector<Life *> &Farm::getLifes() const {
    return lifes;
}

const vector<Entity *> &Farm::getEntities() const {
    return entities;
}




Chunk * Farm::getChunkAt(int chunkX, int chunkY) {
    if (chunkX < 0 || chunkX >= TILE_COUNT_WIDTH || chunkY < 0 || chunkY >= TILE_COUNT_HEIGHT) {
        std::cout << "ERROR, REQUESTED WRONG CHUNK => X: " << chunkX << " Y: " << chunkY << std::endl;
        return chunks.at(0).at(0);
    }




    return chunks.at(chunkX).at(chunkY);
}


Tile * Farm::getTileAt(int tileX, int tileY) {
    if (tileX < 0 || tileX >= TILE_COUNT_WIDTH || tileY < 0 || tileY >= TILE_COUNT_HEIGHT) {
        std::cout << "ERROR, REQUESTED WRONG TILE => X: " << tileX << " Y: " << tileY << std::endl;
        return getChunkAt(0, 0)->getTileAt(0, 0);
    }

    Point chunkPosition = Point(tileX / TILE_PER_CHUNK, tileY / TILE_PER_CHUNK);

    return getChunkAt(chunkPosition.getX(), chunkPosition.getY())->getTileAt(tileX, tileY);
}


std::vector<Life *> Farm::fetchLifes() {
    std::vector<Life *> foundLifes;
    for (int it = 0; it < CHUNK_COUNT_WIDTH; it++) {
        for (int jt = 0; jt < CHUNK_COUNT_HEIGHT; jt++) {
            Chunk * chunk = getChunkAt(it, jt);

            std::vector<Life *> chunkCreatures = chunk->getLifes();

            foundLifes.insert(foundLifes.begin(), chunkCreatures.begin(), chunkCreatures.end());
        }
    }

    return foundLifes;
}

DataAnalyser *Farm::getDataAnalyser() const {
    return dataAnalyser;
}
