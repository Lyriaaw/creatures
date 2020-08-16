//
// Created by Amalric Lombard de Buffières on 7/17/20.
//

#include "Farm.h"

#include <iostream>
#include <sstream>
#include <iomanip>
#include <thread>
#include "../utils/perlin/PerlinNoise.h"


using namespace std;

Farm::Farm(): lastLostEnergy(0.0){
    tickStart = std::chrono::system_clock::now();
    tickEnd = std::chrono::system_clock::now();
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
            Chunk * chunk = new Chunk(Point(it, jt), nursery);
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
        creatures.push_back(initialLife);
    }

    for (int it = 0; it < INITIAL_FOOD_COUNT; it++) {
        Life * initialLife = nursery->generateVegetalFromRandom();

        float creatureEnergy = initialLife->getEnergyManagement()->getMaxMass() / 2.0;
        initialLife->getEnergyManagement()->setEnergy(creatureEnergy);
        initialLife->setMass(creatureEnergy);

        lifes.push_back(initialLife);
        vegetals.push_back(initialLife);
    }

    availableEnergy = 0.f;
    tickCount = 0;

    sortCreatures();


    generateRandomTerrain(seed);

}


void Farm::Tick(bool paused) {

    generateEntityGrid();


    if (!paused) {
        vegetalisation();
        brainProcessing();
    }




    if (!paused) {
        executeCreaturesActions();
    }





     if (!paused) {
        moveCreatures();
        populationControl();
    }




    if (!paused) {
        aTickHavePassed();
        statistics();
    }
    sortCreatures();


    tickEnd = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_time = tickEnd - tickStart;
    tickStart = std::chrono::system_clock::now();

    if (!paused) {
        double tickTime = elapsed_time.count();
        if (tickCount == 1) {
            dataAnalyser.getTickTime()->addValue(0);
        } else {
            dataAnalyser.getTickTime()->addValue(tickTime);
        }


        dataAnalyser.getTickPerSecond()->addValue(1.0 / tickTime);
    }
}



void Farm::brainProcessing() {

    actionsGrid.clear();
    for (int it = 0; it < CHUNK_COUNT_WIDTH; it++) {
        std::vector<std::vector<ActionDTO>> actionsLine;
        for (int jt = 0; jt < CHUNK_COUNT_HEIGHT; jt++) {
            std::vector<ActionDTO> actionsCase;
            actionsLine.push_back(actionsCase);
        }
        actionsGrid.emplace_back(actionsLine);
    }



    std::chrono::system_clock::time_point start = std::chrono::system_clock::now();


    std::chrono::system_clock::time_point chunkProcessingStart = std::chrono::system_clock::now();
    for (int it = 0; it < lifes.size(); it++) {
        Life *currentLife = lifes.at(it);
        currentLife->processSelectedTiles();
    }
    std::chrono::system_clock::time_point chunkProcessingEnd = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_timeChunk = chunkProcessingEnd - chunkProcessingStart;
    dataAnalyser.getChunkSelection()->addValue(elapsed_timeChunk.count());


    double totalEntityGrid = 0.0;
    double totalSensors = 0.0;
    double totalBrain = 0.0;
    double totalActions = 0.0;

    std::chrono::system_clock::time_point brainProcessingStart = std::chrono::system_clock::now();
    for (int it = 0; it < lifes.size(); it++) {
        Life *currentLife = lifes.at(it);

        std::chrono::system_clock::time_point entityGridStart = std::chrono::system_clock::now();

        std::vector<Entity *> accessibleEntities = getAccessibleEntities(currentLife->getSelectedTiles());
        std::vector<Tile *> accessibleTiles = getAccessibleTiles(currentLife->getSelectedTiles());

        std::chrono::system_clock::time_point entityGridEnd = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_time_entity_grid = entityGridEnd - entityGridStart;

        totalEntityGrid += elapsed_time_entity_grid.count();




        std::chrono::system_clock::time_point sensorProcessingStart = std::chrono::system_clock::now();

        currentLife->processSensors(accessibleEntities, accessibleTiles);

        std::chrono::system_clock::time_point sensorProcessingEnd = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_time_sensorProcessing = sensorProcessingEnd - sensorProcessingStart;
        totalSensors += elapsed_time_sensorProcessing.count();




        std::chrono::system_clock::time_point processBrainStart = std::chrono::system_clock::now();

        currentLife->processBrain();

        std::chrono::system_clock::time_point processBrainEnd = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_time_process_brain = processBrainEnd - processBrainStart;
        totalBrain += elapsed_time_process_brain.count();




        std::chrono::system_clock::time_point actionsStart = std::chrono::system_clock::now();


        std::vector<ActionDTO> currentCreatureActions = currentLife->executeExternalActions(accessibleEntities);

        Point currentLifePosition = currentLife->getEntity()->getPosition();
        Point chunkPosition = currentLifePosition.getSimpleCoordinates();
        std::vector<ActionDTO> * currentActionCase = &actionsGrid.at(chunkPosition.getX()).at(chunkPosition.getY());
        currentActionCase->insert(currentActionCase->end(), currentCreatureActions.begin(), currentCreatureActions.end());

        std::chrono::system_clock::time_point actionsEnd = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_time_actions = actionsEnd - actionsStart;
        totalActions += elapsed_time_actions.count();


    }

    std::chrono::system_clock::time_point brainProcessingEnd = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_timeBrain = brainProcessingEnd - brainProcessingStart;
    dataAnalyser.getBrainProcessing()->addValue(totalBrain);


    dataAnalyser.getSensorProcessing()->addValue(totalSensors);
    dataAnalyser.getExternalActions()->addValue(totalActions);
    dataAnalyser.getTotalGridGeneration()->addValue(totalEntityGrid);




    std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_time = end - start;
    dataAnalyser.getBrainProcessingTime()->addValue(elapsed_time.count());
}


void Farm::moveCreatures() {
    std::chrono::system_clock::time_point start = std::chrono::system_clock::now();

    std::vector<Entity* > newEntities;

    for (int it = 0; it < lifes.size(); it++) {
        Life *currentLife = lifes.at(it);
        Point entityPoint = currentLife->getEntity()->getPosition();
        Point tilePoint = entityPoint.getTileCoordinates();

        std::vector<Entity* > producedEntities = currentLife->executeInternalActions();
        newEntities.insert(newEntities.begin(), producedEntities.begin(), producedEntities.end());

        double releasedHeat = currentLife->giveawayEnergy();
        getTileAt(tilePoint.getX(), tilePoint.getY())->addHeat(releasedHeat);

        if (currentLife->getEnergyManagement()->getEnergy() <= 0) {
            this->lifesToDelete.emplace_back(currentLife);
        }

    }

    removeDeletedEntities();

    std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_time = end - start;
    dataAnalyser.getMoveCreaturesTime()->addValue(elapsed_time.count());
}



void Farm::executeCreaturesActions() {
    std::chrono::system_clock::time_point start = std::chrono::system_clock::now();

    for (int it = 0; it < CHUNK_COUNT_WIDTH; it++) {
        for (int jt = 0; jt < CHUNK_COUNT_HEIGHT; jt++) {

            getChunkAt(it, jt)->setLifes(chunkLifeGrid.at(it).at(jt));
            getChunkAt(it, jt)->setEntities(chunkEntityGrid.at(it).at(jt));
            getChunkAt(it, jt)->setActions(actionsGrid.at(it).at(jt));

        }
    }

    double totalLostEnergy(0.0);
    for (int it = 0; it < CHUNK_COUNT_WIDTH; it++) {
        for (int jt = 0; jt < CHUNK_COUNT_HEIGHT; jt++) {
            Chunk * chunk = getChunkAt(it, jt);
            chunk->executeCreaturesActions();

            std::vector<Life *> currentLifesAdded = chunk->getLifesAdded();
            std::vector<Life *> currentLifesToDelete = chunk->getLifesToDelete();

            std::vector<Entity *> currentEntityAdded = chunk->getEntityAdded();
            std::vector<Entity *> currentEntityToDelete = chunk->getEntityToDelete();

            lifes.insert(lifes.end(), currentLifesAdded.begin(), currentLifesAdded.end());
            lifesAdded.insert(lifesAdded.end(), currentLifesAdded.begin(), currentLifesAdded.end());
            lifesToDelete.insert(lifesToDelete.end(), currentLifesToDelete.begin(), currentLifesToDelete.end());
            entities.insert(entities.end(), currentEntityAdded.begin(), currentEntityAdded.end());
            entityAdded.insert(entityAdded.end(), currentEntityAdded.begin(), currentEntityAdded.end());
            entityToDelete.insert(entityToDelete.end(), currentEntityToDelete.begin(), currentEntityToDelete.end());

        }
    }

    lastLostEnergy = totalLostEnergy;


    removeDeletedEntities();

    actions.clear();
//    dataAnalyser.getNaturalMatings()->addValue(naturalMatingCount);

//    int totalActions = captureGroundActions + captureHeatActions + duplicateActions + mateActions + eatActions;
//
//    dataAnalyser.getTotalActions()->addValue(totalActions);
//    dataAnalyser.getCaptureGroundActions()->addValue(captureGroundActions);
//    dataAnalyser.getCaptureHeatActions()->addValue(captureHeatActions);
//    dataAnalyser.getDuplicateActions()->addValue(duplicateActions);
//    dataAnalyser.getMateActions()->addValue(mateActions);
//    dataAnalyser.getEatActions()->addValue(eatActions);



    std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_time = end - start;
    dataAnalyser.getExecuteActionsTime()->addValue(elapsed_time.count());
}

void Farm::handleCaptureHeat(Life * life, ActionDTO action) {

}

void Farm::handleCaptureGround(Life * life, ActionDTO action) {
    int chunkReach = life->getEntity()->getSize() / 5.0;

    chunkReach = std::max(double(chunkReach), 1.0);


    double totalGround = 0.0;
    int ratioSize = (chunkReach * 2) + 1;
    double ratios[ratioSize * ratioSize];
    double test[2][2] = {{1, 1}, {1, 1}};

    Point entityPoint = life->getEntity()->getPosition();
    Point tilePoint = entityPoint.getTileCoordinates();

    for (int it = -chunkReach; it <= chunkReach; it++) {
        for (int jt = -chunkReach; jt <= chunkReach; jt++) {
            int currentTileX = tilePoint.getX() + it;
            int currentTileY = tilePoint.getY() + jt;

            if (currentTileX < 0 || currentTileX >= TILE_COUNT_WIDTH || currentTileY < 0 || currentTileY >= TILE_COUNT_HEIGHT)
                continue;

            totalGround += getTileAt(currentTileX, currentTileY)->getGround();
        }
    }

    for (int it = -chunkReach; it <= chunkReach; it++) {
        for (int jt = -chunkReach; jt <= chunkReach; jt++) {
            int currentTileX = tilePoint.getX() + it;
            int currentTileY = tilePoint.getY() + jt;

            if (currentTileX < 0 || currentTileX >= TILE_COUNT_WIDTH || currentTileY < 0 || currentTileY >= TILE_COUNT_HEIGHT)
                continue;

            Tile * tile = getTileAt(currentTileX, currentTileY);
            double ratio = tile->getGround() / totalGround;
            int currentIndex = ((it + chunkReach) * ratioSize) + (jt + chunkReach);
            ratios[currentIndex] = ratio;
        }
    }
//
    double totalAimedEnergy = action.getValue() * totalGround;

    double totalCollectedEnergy = 0;
    for (int it = -chunkReach; it <= chunkReach; it++) {
        for (int jt = -chunkReach; jt <= chunkReach; jt++) {
            int currentTileX = tilePoint.getX() + it;
            int currentTileY = tilePoint.getY() + jt;

            if (currentTileX < 0 || currentTileX >= TILE_COUNT_WIDTH || currentTileY < 0 || currentTileY >= TILE_COUNT_HEIGHT)
                continue;

            int currentIndex = ((it + chunkReach) * ratioSize) + (jt + chunkReach);
            double currentRatio = ratios[currentIndex] * 0.01;
            double tileEnergy = getTileAt(currentTileX, currentTileY)->getGround();
            double tileCollectedEnergy = (currentRatio * totalAimedEnergy);

            if (tileEnergy - tileCollectedEnergy < 0) {
                std::cout << "Error while removing from ground: " << tileEnergy - tileCollectedEnergy << std::endl;
            }

            totalCollectedEnergy += tileCollectedEnergy;
            getTileAt(currentTileX, currentTileY)->setGround(tileEnergy - tileCollectedEnergy);
        }
    }

//    if (totalCollectedEnergy == 0) {
//    }
    double wastedEnergy = life->addEnergy(totalCollectedEnergy);


    getTileAt(tilePoint.getX(), tilePoint.getY())->addHeat(wastedEnergy);


}


bool Farm::handleDuplication(Life * life) {

    bool fatherCanReproduce = life->getEntity()->getMass() > life->getEnergyManagement()->getMaxMass() / 2.f;

    if (!fatherCanReproduce) {
//        std::cout << "Father cannot reproduce" << std::endl;
        return false;
    }
//
    Life * child = this->nursery->Mate(life, nullptr);

    double givenEnergyToChildGoal = child->getEnergyManagement()->getMaxMass() / 10.f;

    double givenFatherEnergy = std::min(life->getEnergyManagement()->getEnergy(), givenEnergyToChildGoal);

    double actualGivenFatherEnergy = life->getEnergyManagement()->removeEnergy(givenFatherEnergy);

    if (givenFatherEnergy != actualGivenFatherEnergy) {
        std::cout << "Wrong energy given" << std::endl;
    }

    double totalGivenEnergy = actualGivenFatherEnergy;

    if (totalGivenEnergy > givenEnergyToChildGoal / 20.0) {
        child->getEnergyManagement()->setEnergy(totalGivenEnergy / 2.0);
        child->setMass(totalGivenEnergy / 2.0);
        lifes.emplace_back(child);
        lifesAdded.emplace_back(child);

        vegetals.emplace_back(child);

        return true;
    }


    Point childCoordinate = child->getEntity()->getPosition();
    Point tileChildPosition = childCoordinate.getTileCoordinates();

    getTileAt(tileChildPosition.getX(), tileChildPosition.getY())->addGround(totalGivenEnergy);


    if (life->getEnergyManagement()->getEnergy() <= 0) {
        lifesToDelete.emplace_back(life);
    }

//    if (givenMotherEnergy + givenFatherEnergy == 0) {
//        std::cout << "New child " << child->getCreature()->getId() << " Energy: " << givenMotherEnergy + givenFatherEnergy << std::endl;
//    } else {
//        std::cout << "New Child " << std::endl;
//
//    }




    return false;
}





bool Farm::handleMating(Life * father, int entityId) {
    Life * foundLife = getLifeFromId(entityId);
    if (foundLife == nullptr) {
        return false;
    }

    bool fatherCanReproduce = father->getEnergyManagement()->getEnergy() > father->getEntity()->getMass() / 2.f;
    bool motherCanReproduce = foundLife->getEnergyManagement()->getEnergy() > father->getEntity()->getMass() / 2.f;

    if (!fatherCanReproduce || !motherCanReproduce) {
        return false;
    }
//
    Life * child = this->nursery->Mate(father, foundLife);

    double givenEnergyToChildGoal = child->getEnergyManagement()->getMaxMass() / 10.0;

    double givenFatherEnergy = std::min(father->getEnergyManagement()->getEnergy(), givenEnergyToChildGoal / 2.0);
    double givenMotherEnergy = std::min(foundLife->getEnergyManagement()->getEnergy(), givenEnergyToChildGoal / 2.0);

    double actualGivenFatherEnergy = father->getEnergyManagement()->removeEnergy(givenFatherEnergy);
    double actualGivenMotherEnergy = foundLife->getEnergyManagement()->removeEnergy(givenMotherEnergy);

    if (givenFatherEnergy != actualGivenFatherEnergy || givenMotherEnergy != actualGivenMotherEnergy) {
        std::cout << "Wrong energy given" << std::endl;
    }

    double totalGivenEnergy = actualGivenFatherEnergy + actualGivenMotherEnergy;

    if (totalGivenEnergy > givenEnergyToChildGoal / 2.0) {
        child->getEnergyManagement()->setEnergy(totalGivenEnergy / 2.0);
        child->setMass(totalGivenEnergy / 2.0);
        lifes.emplace_back(child);
        lifesAdded.emplace_back(child);

        creatures.emplace_back(child);

        return true;
    }


    Point childCoordinate = child->getEntity()->getPosition();
    Point tileChildPosition = childCoordinate.getTileCoordinates();

    getTileAt(tileChildPosition.getX(), tileChildPosition.getY())->addGround(totalGivenEnergy);


    if (father->getEnergyManagement()->getEnergy() <= 0) {
        lifesToDelete.emplace_back(father);
    }
    if (foundLife->getEnergyManagement()->getEnergy() <= 0) {
        lifesToDelete.emplace_back(foundLife);
    }

//    if (givenMotherEnergy + givenFatherEnergy == 0) {
//        std::cout << "New child " << child->getCreature()->getId() << " Energy: " << givenMotherEnergy + givenFatherEnergy << std::endl;
//    } else {
//        std::cout << "New Child " << std::endl;
//
//    }




    return false;
}



void Farm::populationControl() {
    std::chrono::system_clock::time_point start = std::chrono::system_clock::now();

    if (this->lifes.size() > int(INITIAL_CREATURE_COUNT / 2) - (INITIAL_CREATURE_COUNT * 0.05)) {
        std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_time = end - start;
        dataAnalyser.getPopulationControlTime()->addValue(elapsed_time.count());
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
    dataAnalyser.getPopulationControlTime()->addValue(elapsed_time.count());
}

void Farm::vegetalisation() {
    std::chrono::system_clock::time_point start = std::chrono::system_clock::now();

    std::thread climateThreads[CHUNK_COUNT_WIDTH * CHUNK_COUNT_HEIGHT];

    for (int it = 0; it < CHUNK_COUNT_WIDTH; it++) {
        for (int jt = 0; jt < CHUNK_COUNT_HEIGHT; jt++) {
            auto f = [](Chunk * chunk) {
                chunk->processClimate();
            };

            int index = (it * CHUNK_COUNT_HEIGHT) + jt;

            climateThreads[index] = std::thread(f, chunks.at(it).at(jt));
        }
    }


    for (int it = 0; it < CHUNK_COUNT_WIDTH * CHUNK_COUNT_HEIGHT; it++) {
        climateThreads[it].join();
    }





    std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_time = end - start;
    dataAnalyser.getVegetalisationTime()->addValue(elapsed_time.count());
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



void Farm::statistics() {
    std::chrono::system_clock::time_point start = std::chrono::system_clock::now();


    std::vector<Life *> sortedLife = getScoreSortedCreatures();
//    std::vector<BrainConnector *> sortedConnectors = connectors;
    int populationSize = sortedLife.size();

    dataAnalyser.getPopulation()->addValue(populationSize);

    double totalPopulationScore = 0.0;
    for (int it = 0; it < populationSize; it++) {
        totalPopulationScore += sortedLife.at(it)->getEntity()->getAge();
    }

    double averagePopulationAge = totalPopulationScore / double(populationSize);

    double maxScore = sortedLife.at(0)->getEntity()->getAge();

    double firstQuartileScore = sortedLife.at(populationSize / 4)->getEntity()->getAge();
    double median = sortedLife.at(populationSize / 2)->getEntity()->getAge();
    double lastQuartileScore = sortedLife.at((3 * populationSize) / 4)->getEntity()->getAge();


    dataAnalyser.getAverageScore()->addValue(averagePopulationAge);
    dataAnalyser.getBestScore()->addValue(maxScore);
    dataAnalyser.getFirstQuartileScore()->addValue(firstQuartileScore);
    dataAnalyser.getMedianScore()->addValue(median);
    dataAnalyser.getLastQuartileScore()->addValue(lastQuartileScore);




    double totalCreaturesEnergy = 0.f;
    double totalCreaturesMass = 0.f;
    double totalFoodsMass = 0.f;

    for (int it = 0; it < lifes.size(); it++) {
        Life * currentLife = lifes.at(it);
        totalCreaturesEnergy += currentLife->getEnergyManagement()->getEnergy();
        totalCreaturesMass += currentLife->getEntity()->getMass();
    }

    for (int it = 0; it < entities.size(); it++) {
        Entity * entity = entities.at(it);
        totalFoodsMass += entity->getMass();
    }


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

    std::cout << "Tick: " << tickCount << " Total: " << totalEnergy << " Difference: " << totalEnergy - dataAnalyser.getTotalEnergy()->getLastValue() << " Ground: " << totalGround - dataAnalyser.getGroundEnergy()->getLastValue() << " Lost: " << lastLostEnergy << std::endl;

    dataAnalyser.getTotalEnergy()->addValue(totalEnergy);
    dataAnalyser.getAvailableEnergy()->addValue(availableEnergy);
    dataAnalyser.getFoodEnergy()->addValue(totalFoodsMass);
    dataAnalyser.getCreaturesMass()->addValue(totalCreaturesMass);
    dataAnalyser.getCreaturesEnergy()->addValue(totalCreaturesEnergy);
    dataAnalyser.getHeatEnergy()->addValue(totalHeat);
    dataAnalyser.getGroundEnergy()->addValue(totalGround);
    dataAnalyser.getEnergyToAdd()->addValue(totalToAdd);




    double totalTime = 0.0;
    totalTime += dataAnalyser.getEntityGridTime()->getLastValue();
    totalTime += dataAnalyser.getBrainProcessingTime()->getLastValue();
    totalTime += dataAnalyser.getBrainOutputsTime()->getLastValue();
    totalTime += dataAnalyser.getPrepareActionsTime()->getLastValue();
    totalTime += dataAnalyser.getExecuteActionsTime()->getLastValue();
    totalTime += dataAnalyser.getMoveCreaturesTime()->getLastValue();
    totalTime += dataAnalyser.getPopulationControlTime()->getLastValue();
    totalTime += dataAnalyser.getVegetalisationTime()->getLastValue();

    std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_time = end - start;
    double statisticsTime = elapsed_time.count();

    dataAnalyser.getStatisticsTime()->addValue(statisticsTime);
    totalTime += statisticsTime;

    dataAnalyser.getTotalTime()->addValue(totalTime);
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
    dataAnalyser.getEntityGridTime()->addValue(elapsed_time.count());
}

void Farm::removeDeletedEntities() {
    std::vector<Life *> newLifes;
    for (int it = 0; it < lifes.size(); it++) {

        bool found = false;
        int foundIndex = -1;
        for (int jt = 0; jt < lifesToDelete.size(); jt++) {
            if (lifes.at(it)->getEntity()->getId() == lifesToDelete.at(jt)->getEntity()->getId()) {
                found = true;
                foundIndex = jt;
            }
        }

        if (found) {
            Life * currentLife = lifes.at(it);
            Point currentLifePosition = currentLife->getEntity()->getPosition();
            Point currentLifeTilePosition = currentLifePosition.getTileCoordinates();

            getTileAt(int(currentLifeTilePosition.getX()), int(currentLifeTilePosition.getY()))->addGround(currentLife->getEntity()->getMass());
            getTileAt(int(currentLifeTilePosition.getX()), int(currentLifeTilePosition.getY()))->addHeat(currentLife->getEnergyManagement()->getEnergy());
        } else {
            newLifes.emplace_back(lifes.at(it));
        }
    }

    lifes.clear();
    lifes = newLifes;

    std::vector<Entity *> newEntities;
    for (int it = 0; it < entities.size(); it++) {

        if (entities.at(it)->getMass() > 0) {
            newEntities.emplace_back(entities.at(it));
        }
    }

    entities.clear();
    entities = newEntities;


    std::vector<Life *> newCreatures;
    for (int it = 0; it < creatures.size(); it++) {

        bool found = false;
        int foundIndex = -1;
        for (int jt = 0; jt < lifesToDelete.size(); jt++) {
            if (creatures.at(it)->getEntity()->getId() == lifesToDelete.at(jt)->getEntity()->getId()) {
                found = true;
                foundIndex = jt;
            }
        }

        if (!found) {
            newCreatures.emplace_back(creatures.at(it));
        }
    }

    creatures = newCreatures;

    std::vector<Life *> newVegetals;
    for (int it = 0; it < vegetals.size(); it++) {

        bool found = false;
        int foundIndex = -1;
        for (int jt = 0; jt < lifesToDelete.size(); jt++) {
            if (vegetals.at(it)->getEntity()->getId() == lifesToDelete.at(jt)->getEntity()->getId()) {
                found = true;
                foundIndex = jt;
            }
        }

        if (!found) {
            newVegetals.emplace_back(vegetals.at(it));
        }
    }

    vegetals = newVegetals;







}

std::vector<Entity *> Farm::getAccessibleEntities(std::vector<Point> selectedTiles) {
    std::vector<Entity *> accessibleEntities;
    for (int jt = 0; jt < selectedTiles.size(); jt++) {
        Point currentTiles = selectedTiles.at(jt);

        std::vector<Entity *> chunkEntities = entityGrid.at(currentTiles.getX()).at(currentTiles.getY());

        accessibleEntities.insert(accessibleEntities.end(), chunkEntities.begin(), chunkEntities.end());
    }
    return accessibleEntities;

}
std::vector<Tile *> Farm::getAccessibleTiles(std::vector<Point> selectedChunks) {
    std::vector<Tile *> accessibleTiles;
    for (int jt = 0; jt < selectedChunks.size(); jt++) {
        Point currentTilePoint = selectedChunks.at(jt);

        Tile * currentTile = getTileAt(currentTilePoint.getX(), currentTilePoint.getY());

        accessibleTiles.emplace_back(currentTile);
    }
    return accessibleTiles;

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



Life * Farm::getLifeFromId(int id) {
    for (int it = 0; it < this->lifes.size(); it++) {
        if (this->lifes.at(it)->getEntity()->getId() == id) {
            return this->lifes.at(it);
        }
    }

    return nullptr;
}

Entity * Farm::getEntityFromId(int id) {
    for (int it = 0; it < this->lifes.size(); it++) {
        if (this->lifes.at(it)->getEntity()->getId() == id) {
            return this->lifes.at(it)->getEntity();
        }
    }

    for (int it = 0; it < this->entities.size(); it++) {
        if (this->entities.at(it)->getId() == id) {
            return this->entities.at(it);
        }
    }


    return nullptr;
}










CreatureNursery *Farm::getNursery() const {
    return nursery;
}


void Farm::addLife(Life * life) {
    this->lifes.push_back(life);
}


void Farm::clearAddedLifes() {
    this->lifesAdded.clear();
}

void Farm::clearAddedEntities() {
    this->entityAdded.clear();
}
void Farm::clearToDeleteLifes() {
    this->lifesToDelete.clear();
}

void Farm::clearToDeleteEntities() {
    this->entityToDelete.clear();
}



std::string Farm::getHumanReadableEnergy(float givenEnergy) {
    std::stringstream givenEnergyStream;


    if (givenEnergy > 1000000000.f) {
        givenEnergyStream << std::fixed << std::setprecision(2) << givenEnergy / 1000000000.f;
        givenEnergyStream << " GA";
        return givenEnergyStream.str();
    }

    if (givenEnergy > 1000000.f) {
        givenEnergyStream << std::fixed << std::setprecision(2) << givenEnergy / 1000000.f;
        givenEnergyStream << " mA";
        return givenEnergyStream.str();
    }

    if (givenEnergy > 1000.f) {
        givenEnergyStream << std::fixed << std::setprecision(2) << givenEnergy / 1000.f;
        givenEnergyStream << " kA";
        return givenEnergyStream.str();
    }


    givenEnergyStream << givenEnergy;
    givenEnergyStream << " A";

    return givenEnergyStream.str();
}

const DataAnalyser &Farm::getDataAnalyser() const {
    return dataAnalyser;
}

void Farm::setDataAnalyser(const DataAnalyser &dataAnalyser) {
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

const vector<Life *> &Farm::getLifesAdded() const {
    return lifesAdded;
}


const vector<Life *> &Farm::getLifesToDelete() const {
    return lifesToDelete;
}

const vector<Entity *> &Farm::getEntityAdded() const {
    return entityAdded;
}

const vector<Entity *> &Farm::getEntityToDelete() const {
    return entityToDelete;
}

const vector<Life *> &Farm::getCreatures() const {
    return creatures;
}

const vector<Life *> &Farm::getVegetals() const {
    return vegetals;
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

void Farm::setLifes(const vector<Life *> &lifes) {
    Farm::lifes = lifes;
}

void Farm::setEntities(const vector<Entity *> &entities) {
    Farm::entities = entities;
}
