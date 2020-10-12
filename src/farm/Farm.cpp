//
// Created by Amalric Lombard de Buffières on 7/17/20.
//

#include "Farm.h"

#include <iostream>
#include <sstream>
#include <iomanip>


using namespace std;

Farm::Farm(){
    tickStart = std::chrono::system_clock::now();
    tickEnd = std::chrono::system_clock::now();
}


void Farm::InitFromRandom() {
    random_device rd;
    mt19937 mt(rd());
    uniform_real_distribution<double> distWidth(0, FARM_WIDTH);
    uniform_real_distribution<double> distHeight(0, FARM_HEIGHT);

    map = new Map();
    map->initRandomMap();


    std::vector<std::vector<std::vector<Entity *>>> testEntites;

    for (int it = 0; it < CHUNK_COUNT_WIDTH; it++) {
        std::vector<std::vector<Entity *>> line;
        for (int jt = 0; jt < CHUNK_COUNT_HEIGHT; jt++) {
            std::vector<Entity *> currentChunk;
            line.push_back(currentChunk);
        }
        testEntites.emplace_back(line);
    }

    entityGrid = testEntites;

    std::uniform_real_distribution<float> distMovement(-1, 1);
    nursery = new CreatureNursery();
    for (int it = 0; it < INITIAL_CREATURE_COUNT; it++) {
        Life * initialLife = nursery->generateCreatureFromRandom();

        float creatureEnergy = initialLife->getEnergyCenter()->getMaxMass() / 2.0;
        initialLife->getEntity()->setMass(creatureEnergy);

        lifes.push_back(initialLife);
    }

    for (int it = 0; it < INITIAL_FOOD_COUNT; it++) {
        int x = distWidth(mt);
        int y = distHeight(mt);

        Point point(x, y);


//        float foodSize = ((rand() % 300) / 100.f) + 2;
        float foodSize = 2;

        Food * entity = new Food(point, foodSize);
        entity->setMass(2 * MASS_TO_SIZE_RATIO);
        entities.push_back(entity);
    }

    availableEnergy = 0.f;
    tickCount = 0;

    sortCreatures();
}


void Farm::Tick(bool paused) {

    generateEntityGrid();


    if (!paused) {
        multithreadBrainProcessing();
        executeCreaturesActions();
        moveCreatures();
        vegetalisation();
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

void Farm::multithreadBrainProcessing() {
    std::chrono::system_clock::time_point start = std::chrono::system_clock::now();

    std::thread chunkThreads[lifes.size()];



    for (int it = 0; it < lifes.size(); it++) {

        auto f = [](Life * life, Farm * farm) {

            life->processSelectedChunks();


            std::vector<Entity *> accessibleEntities = farm->getAccessibleEntities(life->getSelectedChunks());
            std::vector<Tile *> accessibleTiles = farm->getAccessibleTiles(life->getSelectedChunks());
            life->processSensors(accessibleEntities, accessibleTiles);


            life->processBrain();


//
            std::vector<ActionDTO> currentCreatureActions = life->executeExternalActions(accessibleEntities);

            farm->addActions(currentCreatureActions);


        };

        Life *currentLife = lifes.at(it);

        int index = it;
        chunkThreads[index] = std::thread(f, currentLife, this);
    }

    for (int it = 0; it < lifes.size(); it++) {
        chunkThreads[it].join();
    }



    std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_time = end - start;
    dataAnalyser.getBrainProcessingTime()->addValue(elapsed_time.count());
}

void Farm::addActions(std::vector<ActionDTO> givenActions) {
    std::lock_guard<std::mutex> guard(actions_mutex);
    actions.insert(actions.end(), givenActions.begin(), givenActions.end());
}



void Farm::brainProcessing() {
    std::chrono::system_clock::time_point start = std::chrono::system_clock::now();


    std::chrono::system_clock::time_point chunkProcessingStart = std::chrono::system_clock::now();
    for (int it = 0; it < lifes.size(); it++) {
        Life *currentLife = lifes.at(it);
        currentLife->processSelectedChunks();
    }
    std::chrono::system_clock::time_point chunkProcessingEnd = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_timeChunk = chunkProcessingEnd - chunkProcessingStart;
    dataAnalyser.getChunkSelection()->addValue(elapsed_timeChunk.count());



    std::chrono::system_clock::time_point sensorProcessingStart = std::chrono::system_clock::now();
    for (int it = 0; it < lifes.size(); it++) {
        Life *currentLife = lifes.at(it);

        std::vector<Entity *> accessibleEntities = getAccessibleEntities(currentLife->getSelectedChunks());
        std::vector<Tile *> accessibleTiles = getAccessibleTiles(currentLife->getSelectedChunks());
        currentLife->processSensors(accessibleEntities, accessibleTiles);
    }
    std::chrono::system_clock::time_point sensorProcessingEnd = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_timeSensor = sensorProcessingEnd - sensorProcessingStart;
    dataAnalyser.getSensorProcessing()->addValue(elapsed_timeSensor.count());




    std::chrono::system_clock::time_point brainProcessingStart = std::chrono::system_clock::now();

    for (int it = 0; it < lifes.size(); it++) {
        Life *currentLife = lifes.at(it);

        currentLife->processBrain();
    }
    std::chrono::system_clock::time_point brainProcessingEnd = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_timeBrain = brainProcessingEnd - brainProcessingStart;
    dataAnalyser.getBrainProcessing()->addValue(elapsed_timeBrain.count());


    std::chrono::system_clock::time_point externalActionsStart = std::chrono::system_clock::now();
    for (int it = 0; it < lifes.size(); it++) {
        Life *currentLife = lifes.at(it);

        std::vector<Entity *> accessibleEntities = getAccessibleEntities(currentLife->getSelectedChunks());

        std::vector<ActionDTO> currentCreatureActions = currentLife->executeExternalActions(accessibleEntities);
        actions.insert(actions.end(), currentCreatureActions.begin(), currentCreatureActions.end());
    }
    std::chrono::system_clock::time_point externalActionsEnd = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_timeActions = externalActionsEnd - externalActionsStart;
    dataAnalyser.getExternalActions()->addValue(elapsed_timeActions.count());



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
        map->getTileAt(tilePoint.getX(), tilePoint.getY())->addHeat(releasedHeat);

        if (currentLife->getEnergyCenter()->getAvailableEnergy() <= 0) {
            map->getTileAt(tilePoint.getX(), tilePoint.getY())->addGround(currentLife->getEntity()->getMass());
            map->getTileAt(tilePoint.getX(), tilePoint.getY())->addGround(currentLife->getEnergyCenter()->getWastedEnergy());
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

    int naturalMatingCount = 0;
    for (int it = 0; it < actions.size(); it++) {
        ActionDTO actionDto = actions.at(it);


        Life * performer = getLifeFromId(actionDto.getPerformerId());
        Entity * subject = getEntityFromId(actionDto.getSubjectId());

        if (subject->getMass() <= 0 || performer->getEntity()->getMass() <= 0) {
            continue;
        }

        if (actionDto.getType() == "EAT") {
            double wastedEnergy = performer->addEnergy(subject->getMass());
            subject->setMass(0.0);

            Point performerPoint = performer->getEntity()->getPosition();
            Point tilePoint = performerPoint.getTileCoordinates();

            map->getTileAt(tilePoint.getX(), tilePoint.getY())->addGround(wastedEnergy);
            entityToDelete.emplace_back(subject);
        }

        if (actionDto.getType() == "MATE") {
            bool success = handleMating(performer, subject->getId());

            if (success)
                naturalMatingCount++;

        }

    }
    removeDeletedEntities();

    actions.clear();
    dataAnalyser.getNaturalMatings()->addValue(naturalMatingCount);

    std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_time = end - start;
    dataAnalyser.getExecuteActionsTime()->addValue(elapsed_time.count());
}

bool Farm::handleMating(Life * father, int entityId) {
    Life * foundLife = getLifeFromId(entityId);
    if (foundLife == nullptr) {
        return false;
    }

    bool fatherCanReproduce = father->getEntity()->getMass() > father->getEnergyCenter()->getMaxMass() / 2.f;
    bool motherCanReproduce = foundLife->getEntity()->getMass() > father->getEnergyCenter()->getMaxMass() / 2.f;

    if (!fatherCanReproduce || !motherCanReproduce) {
        return false;
    }
//
    Life * child = this->nursery->Mate(father, foundLife);

    double givenEnergyToChildGoal = child->getEnergyCenter()->getMaxMass() / 4.f;

    double givenFatherEnergy = std::min(father->getEntity()->getMass() / 2.0, givenEnergyToChildGoal / 2.0);
    double givenMotherEnergy = std::min(foundLife->getEntity()->getMass() / 2.0, givenEnergyToChildGoal / 2.0);

    double actualGivenFatherEnergy = father->getEntity()->removeMass(givenFatherEnergy);
    double actualGivenMotherEnergy = foundLife->getEntity()->removeMass(givenMotherEnergy);

    if (givenFatherEnergy != actualGivenFatherEnergy || givenMotherEnergy != actualGivenMotherEnergy) {
        std::cout << "Wrong energy given" << std::endl;
    }

    double totalGivenEnergy = actualGivenFatherEnergy + actualGivenMotherEnergy;

    if (totalGivenEnergy > givenEnergyToChildGoal / 2.0) {
        child->getEntity()->setMass(totalGivenEnergy / 2.0);
        child->getEnergyCenter()->setAvailableEnergy(totalGivenEnergy / 2.0);
        lifes.emplace_back(child);
        lifesAdded.emplace_back(child);

        return true;
    }


    Point childCoordinate = child->getEntity()->getPosition();
    Point tileChildPosition = childCoordinate.getTileCoordinates();

    map->getTileAt(tileChildPosition.getX(), tileChildPosition.getY())->addGround(totalGivenEnergy);


    if (father->getEntity()->getMass() <= 0) {
        lifesToDelete.emplace_back(father);
    }
    if (foundLife->getEntity()->getMass() <= 0) {
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
        child->getEntity()->setMass(child->getEnergyCenter()->getMaxMass() / 4.f);
        child->getEnergyCenter()->setAvailableEnergy(child->getEnergyCenter()->getMaxMass() / 4.f);
        Entity * childCreature = child->getEntity();

//        float childSpawnX = distWidth(mt);
//        float childSpawnY = distHeight(mt);
//        Point childCreaturePosition = Point(childSpawnX, childSpawnY);
//
//        childCreature->setPosition(childCreaturePosition);

        totalEnergyRemoved += child->getEntity()->getMass() + child->getEnergyCenter()->getAvailableEnergy();


        lifes.emplace_back(child);
        lifesAdded.emplace_back(child);
    }

    map->removeEnergyFromGround(totalEnergyRemoved);



    std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_time = end - start;
    dataAnalyser.getPopulationControlTime()->addValue(elapsed_time.count());
}

void Farm::handleDecay() {
    for (int it = 0; it < entities.size(); it++) {
        Entity * entity = entities.at(it);
        Point position = entity->getPosition();
        Point tileCoordinate = position.getTileCoordinates();
        Tile * tile = map->getTileAt(tileCoordinate.getX(), tileCoordinate.getY());

        if (entity->getMass() < 10) {
            tile->addGround(entity->getMass());
            entity->setMass(0.0);
            entityToDelete.emplace_back(entity);
        } else {
            tile->addGround(1.0);
            entity->removeMass(1.0);
        }

    }
}

void Farm::vegetalisation() {
    std::chrono::system_clock::time_point start = std::chrono::system_clock::now();

    map->processClimate();
    handleDecay();

    random_device rd;
    mt19937 mt(rd());
    uniform_real_distribution<double> distWidth(0, TILE_SIZE);
    uniform_real_distribution<double> distHeight(0, TILE_SIZE);


    for (int it = 0; it < TILE_COUNT_WIDTH; it++) {
        for (int jt = 0; jt < TILE_COUNT_HEIGHT; jt++) {
            Tile * currentTile = map->getTileAt(it, jt);

            float tileX = it * TILE_SIZE;
            float tileY = jt * TILE_SIZE;

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


                //        float foodSize = ((rand() % 300) / 100.f) + 2;
                float foodSize = 2;

                Food * entity = new Food(point, foodSize);
                entity->setMass(2 * MASS_TO_SIZE_RATIO);

                totalEnergyAdded += entity->getMass();


                entities.emplace_back(entity);
                entityAdded.emplace_back(entity);
            }

            currentTile->addGround(-1 * totalEnergyAdded);

        }
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
    double totalCreaturesWasted = 0.f;
    double totalFoodsEnergy = 0.f;

    for (int it = 0; it < lifes.size(); it++) {
        Life * currentLife = lifes.at(it);
        totalCreaturesEnergy += currentLife->getEnergyCenter()->getAvailableEnergy();
        totalCreaturesMass += currentLife->getEntity()->getMass();
        totalCreaturesWasted += currentLife->getEnergyCenter()->getWastedEnergy();
    }

    for (int it = 0; it < entities.size(); it++) {
        Entity * entity = entities.at(it);
        totalFoodsEnergy += entity->getMass();
    }


    double totalHeat = 0.0;
    double totalGround = 0.0;
    for (int it = 0; it < TILE_COUNT_WIDTH; it++) {
        for (int jt = 0; jt < TILE_COUNT_HEIGHT; jt++) {
            totalHeat += map->getTileAt(it, jt)->getHeat();
            totalGround += map->getTileAt(it, jt)->getGround();
        }
    }


    int totalEnergy = availableEnergy + totalFoodsEnergy + totalCreaturesEnergy + totalCreaturesMass + totalCreaturesWasted + totalHeat + totalGround;
    dataAnalyser.getTotalEnergy()->addValue(totalEnergy);
    dataAnalyser.getAvailableEnergy()->addValue(availableEnergy);
    dataAnalyser.getFoodEnergy()->addValue(totalFoodsEnergy);
    dataAnalyser.getCreaturesEnergy()->addValue(totalCreaturesEnergy);
    dataAnalyser.getCreaturesMass()->addValue(totalCreaturesMass);
    dataAnalyser.getCreaturesWastedEnergy()->addValue(totalCreaturesWasted);
    dataAnalyser.getHeatEnergy()->addValue(totalHeat);
    dataAnalyser.getGroundEnergy()->addValue(totalGround);

//    std::cout << "Total: " << totalEnergy << " ";
//    std::cout << "Available: " << totalCreaturesEnergy << " ";
//    std::cout << "Mass: " << totalCreaturesMass << " ";
//    std::cout << "Wasted: " << totalCreaturesWasted << " ";
//    std::cout << std::endl;

//    if (dataAnalyser.getTotalEnergy()->getLastValue() != dataAnalyser.getTotalEnergy()->getSecondToLastValue()) {
//        std::cout << "ERROR : Lost total energy : " << dataAnalyser.getTotalEnergy()->getSecondToLastValue() - dataAnalyser.getTotalEnergy()->getLastValue() << std::endl;
//    }



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

    for (int it = 0; it < CHUNK_COUNT_WIDTH; it++) {
        for (int jt = 0; jt < CHUNK_COUNT_HEIGHT; jt++) {
            entityGrid.at(it).at(jt).clear();
        }
    }

    for (int it = 0; it < lifes.size(); it++) {
        Point simpleCoordinates = lifes.at(it)->getEntity()->getSimpleCoordinates();
        try {
            entityGrid.at(simpleCoordinates.getX()).at(simpleCoordinates.getY()).push_back(lifes.at(it)->getEntity());
        } catch (const std::exception&) {
            std::cout << "Error for simple coordinates: " << simpleCoordinates.getX() << " - " << simpleCoordinates.getY() << std::endl;
        }
    }

    for (int it = 0; it < entities.size(); it++) {
        Point simpleCoordinates = entities.at(it)->getSimpleCoordinates();
        entityGrid.at(simpleCoordinates.getX()).at(simpleCoordinates.getY()).push_back(entities.at(it));
    }



    std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_time = end - start;
    dataAnalyser.getEntityGridTime()->addValue(elapsed_time.count());
}

void Farm::removeDeletedEntities() {
    std::vector<Life *> newLifes;
    for (int it = 0; it < lifes.size(); it++) {

        bool found = false;
        for (int jt = 0; jt < lifesToDelete.size(); jt++) {
            if (lifes.at(it)->getEntity()->getId() == lifesToDelete.at(jt)->getEntity()->getId()) {
                found = true;
            }
        }

        if (!found) {
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
}

std::vector<Entity *> Farm::getAccessibleEntities(std::vector<Point> selectedChunks) {
    std::vector<Entity *> accessibleEntities;
    for (int jt = 0; jt < selectedChunks.size(); jt++) {
        Point currentChunk = selectedChunks.at(jt);

        std::vector<Entity *> chunkEntities = entityGrid.at(currentChunk.getX()).at(currentChunk.getY());

        accessibleEntities.insert(accessibleEntities.end(), chunkEntities.begin(), chunkEntities.end());
    }
    return accessibleEntities;

}
std::vector<Tile *> Farm::getAccessibleTiles(std::vector<Point> selectedChunks) {
    std::vector<Tile *> accessibleTiles;
    for (int jt = 0; jt < selectedChunks.size(); jt++) {
        Point currentTilePoint = selectedChunks.at(jt);

        Tile * currentTile = map->getTileAt(currentTilePoint.getX(), currentTilePoint.getY());

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

Map *Farm::getMap() const {
    return map;
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



