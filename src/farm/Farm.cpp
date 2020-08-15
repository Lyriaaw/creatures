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
        Life * initialLife = nursery->generateVegetalFromRandom();

        float creatureEnergy = initialLife->getBody()->getMaximumMass() / 10.0;
        initialLife->getBody()->setEnergy(creatureEnergy);

        lifes.push_back(initialLife);
    }

    for (int it = 0; it < INITIAL_FOOD_COUNT; it++) {
        int x = distWidth(mt);
        int y = distHeight(mt);

        Point point(x, y);


//        float foodSize = ((rand() % 300) / 100.f) + 2;
        float foodSize = 2;

        Food * entity = new Food(point, foodSize);
        entity->setMass(2000.0);
        entities.push_back(entity);
    }

    availableEnergy = 0.f;
    tickCount = 0;

    sortCreatures();
}


void Farm::Tick(bool paused) {

    generateEntityGrid();


    if (!paused) {
        vegetalisation();
        brainProcessing();
        executeCreaturesActions();
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
    std::chrono::system_clock::time_point start = std::chrono::system_clock::now();


    std::chrono::system_clock::time_point chunkProcessingStart = std::chrono::system_clock::now();
    for (int it = 0; it < lifes.size(); it++) {
        Life *currentLife = lifes.at(it);
        currentLife->processSelectedChunks();
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

        std::vector<Entity *> accessibleEntities = getAccessibleEntities(currentLife->getSelectedChunks());
        std::vector<Tile *> accessibleTiles = getAccessibleTiles(currentLife->getSelectedChunks());

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
        actions.insert(actions.end(), currentCreatureActions.begin(), currentCreatureActions.end());

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
        map->getTileAt(tilePoint.getX(), tilePoint.getY())->addHeat(releasedHeat);

        if (currentLife->getEntity()->getEnergy() <= 0) {
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

        if (performer->getEntity()->getEnergy() <= 0) {
            continue;
        }

        if (subject != nullptr && subject->getMass() <= 0) {
            continue;
        }

        if (actionDto.getType() == "EAT") {
            double wastedEnergy = performer->addEnergy(subject->getMass());
            subject->setEnergy(0.0);

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

        if (actionDto.getType() == "DUPLICATE") {
            bool success = handleDuplication(performer);

            if (success)
                naturalMatingCount++;

        }

        if (actionDto.getType() == "CAPTURE_GROUND") {
            handleCaptureGround(performer, actionDto);
        }

        if (actionDto.getType() == "CAPTURE_HEAT") {
            handleCaptureHeat(performer, actionDto);
        }



    }
    removeDeletedEntities();

    actions.clear();
    dataAnalyser.getNaturalMatings()->addValue(naturalMatingCount);

    std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_time = end - start;
    dataAnalyser.getExecuteActionsTime()->addValue(elapsed_time.count());
}

void Farm::handleCaptureHeat(Life * life, ActionDTO action) {

}

void Farm::handleCaptureGround(Life * life, ActionDTO action) {
    int chunkReach = life->getEntity()->getSize() / 5;

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

            totalGround += map->getTileAt(currentTileX, currentTileY)->getGround();
        }
    }

    for (int it = -chunkReach; it <= chunkReach; it++) {
        for (int jt = -chunkReach; jt <= chunkReach; jt++) {
            int currentTileX = tilePoint.getX() + it;
            int currentTileY = tilePoint.getY() + jt;

            if (currentTileX < 0 || currentTileX >= TILE_COUNT_WIDTH || currentTileY < 0 || currentTileY >= TILE_COUNT_HEIGHT)
                continue;

            Tile * tile = map->getTileAt(currentTileX, currentTileY);
            double ratio = tile->getGround() / totalGround;
            int currentIndex = ((it + chunkReach) * ratioSize) + (jt * chunkReach);
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

            int currentIndex = ((it + chunkReach) * ratioSize) + (jt * chunkReach);
            double currentRatio = ratios[currentIndex] * 0.01;
            double tileEnergy = map->getTileAt(currentTileX, currentTileY)->getGround();
            double tileCollectedEnergy = (currentRatio * totalAimedEnergy);

            if (tileEnergy - tileCollectedEnergy < 0) {
                std::cout << "Error while removing from ground: " << tileEnergy - tileCollectedEnergy << std::endl;
            }

            totalCollectedEnergy += tileCollectedEnergy;
            map->getTileAt(currentTileX, currentTileY)->setGround(tileEnergy - tileCollectedEnergy);
        }
    }

    double heat = life->getEntity()->addEnergy(totalCollectedEnergy);

    map->getTileAt(tilePoint.getX(), tilePoint.getY())->addHeat(heat);

}


bool Farm::handleDuplication(Life * life) {

    bool fatherCanReproduce = life->getEntity()->getEnergy() > life->getEntity()->getMaxEnergy() / 2.f;

    if (!fatherCanReproduce) {
//        std::cout << "Father cannot reproduce" << std::endl;
        return false;
    }
//
    Life * child = this->nursery->Mate(life, nullptr);

    double givenEnergyToChildGoal = child->getEntity()->getMaxEnergy() / 4.f;

    double givenFatherEnergy = std::min(life->getEntity()->getEnergy() / 2.0, givenEnergyToChildGoal / 2.0);

    double actualGivenFatherEnergy = life->getEntity()->removeEnergy(givenFatherEnergy);

    if (givenFatherEnergy != actualGivenFatherEnergy) {
        std::cout << "Wrong energy given" << std::endl;
    }

    double totalGivenEnergy = actualGivenFatherEnergy;

    if (totalGivenEnergy > givenEnergyToChildGoal / 20.0) {
        child->getEntity()->setEnergy(totalGivenEnergy);
        lifes.emplace_back(child);
        lifesAdded.emplace_back(child);

        return true;
    }


    Point childCoordinate = child->getEntity()->getPosition();
    Point tileChildPosition = childCoordinate.getTileCoordinates();

    map->getTileAt(tileChildPosition.getX(), tileChildPosition.getY())->addGround(totalGivenEnergy);


    if (life->getEntity()->getEnergy() <= 0) {
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

    bool fatherCanReproduce = father->getEntity()->getEnergy() > father->getMaximumMass() / 2.f;
    bool motherCanReproduce = foundLife->getEntity()->getEnergy() > father->getMaximumMass() / 2.f;

    if (!fatherCanReproduce || !motherCanReproduce) {
        return false;
    }
//
    Life * child = this->nursery->Mate(father, foundLife);

    double givenEnergyToChildGoal = child->getMaximumMass() / 10.f;

    double givenFatherEnergy = std::min(father->getEntity()->getEnergy() / 2.0, givenEnergyToChildGoal / 2.0);
    double givenMotherEnergy = std::min(foundLife->getEntity()->getEnergy() / 2.0, givenEnergyToChildGoal / 2.0);

    double actualGivenFatherEnergy = father->removeEnergy(givenFatherEnergy);
    double actualGivenMotherEnergy = foundLife->removeEnergy(givenMotherEnergy);

    if (givenFatherEnergy != actualGivenFatherEnergy || givenMotherEnergy != actualGivenMotherEnergy) {
        std::cout << "Wrong energy given" << std::endl;
    }

    double totalGivenEnergy = actualGivenFatherEnergy + actualGivenMotherEnergy;

    if (totalGivenEnergy > givenEnergyToChildGoal / 2.0) {
        child->getEntity()->setEnergy(totalGivenEnergy);
        lifes.emplace_back(child);
        lifesAdded.emplace_back(child);

        return true;
    }


    Point childCoordinate = child->getEntity()->getPosition();
    Point tileChildPosition = childCoordinate.getTileCoordinates();

    map->getTileAt(tileChildPosition.getX(), tileChildPosition.getY())->addGround(totalGivenEnergy);


    if (father->getEntity()->getEnergy() <= 0) {
        lifesToDelete.emplace_back(father);
    }
    if (foundLife->getEntity()->getEnergy() <= 0) {
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
        child->getEntity()->setEnergy(child->getEntity()->getMaxEnergy() / 4.f);
        Entity * childCreature = child->getEntity();

//        float childSpawnX = distWidth(mt);
//        float childSpawnY = distHeight(mt);
//        Point childCreaturePosition = Point(childSpawnX, childSpawnY);
//
//        childCreature->setPosition(childCreaturePosition);

        totalEnergyRemoved += child->getEntity()->getEnergy();


        lifes.emplace_back(child);
        lifesAdded.emplace_back(child);
    }

    map->removeEnergyFromGround(totalEnergyRemoved);



    std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_time = end - start;
    dataAnalyser.getPopulationControlTime()->addValue(elapsed_time.count());
}

void Farm::vegetalisation() {
    std::chrono::system_clock::time_point start = std::chrono::system_clock::now();

    map->processClimate();

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
                entity->setEnergy(2000.0);

                totalEnergyAdded += entity->getEnergy();


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
    double totalFoodsEnergy = 0.f;

    for (int it = 0; it < lifes.size(); it++) {
        Entity * currentEntity = lifes.at(it)->getEntity();
        totalCreaturesEnergy += currentEntity->getEnergy();
    }

    for (int it = 0; it < entities.size(); it++) {
        Entity * entity = entities.at(it);
        totalFoodsEnergy += entity->getEnergy();
    }


    double totalHeat = 0.0;
    double totalGround = 0.0;
    for (int it = 0; it < TILE_COUNT_WIDTH; it++) {
        for (int jt = 0; jt < TILE_COUNT_HEIGHT; jt++) {
            totalHeat += map->getTileAt(it, jt)->getHeat();
            totalGround += map->getTileAt(it, jt)->getGround();
        }
    }


    int totalEnergy = availableEnergy + totalFoodsEnergy + totalCreaturesEnergy + totalHeat + totalGround;
    dataAnalyser.getTotalEnergy()->addValue(totalEnergy);
    dataAnalyser.getAvailableEnergy()->addValue(availableEnergy);
    dataAnalyser.getFoodEnergy()->addValue(totalFoodsEnergy);
    dataAnalyser.getCreaturesEnergy()->addValue(totalCreaturesEnergy);
    dataAnalyser.getHeatEnergy()->addValue(totalHeat);
    dataAnalyser.getGroundEnergy()->addValue(totalGround);




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
        entityGrid.at(simpleCoordinates.getX()).at(simpleCoordinates.getY()).push_back(lifes.at(it)->getEntity());
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

        if (entities.at(it)->getEnergy() > 0) {
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


