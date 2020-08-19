//
// Created by Amalric Lombard de Buffières on 8/15/20.
//

#include <zconf.h>
#include "Chunk.h"
#include "../utils/perlin/PerlinNoise.h"

Chunk::Chunk(Point chunkPosition, CreatureNursery * nursery, DataAnalyser * dataAnalyser): chunkPosition(chunkPosition), nursery(nursery), step("init"), tick(0), dataAnalyser(dataAnalyser) {
    generateNeighbours();

    for (int it = 0; it < TILE_PER_CHUNK; it++) {
        std::vector<std::vector<Entity *>> entityLine;
        for (int jt = 0; jt < TILE_PER_CHUNK; jt++) {
            std::vector<Entity *> entityCase;
            entityLine.push_back(entityCase);
        }
        entityGrid.emplace_back(entityLine);
    }
}

// Main loop
void Chunk::generateEntityGrid() {
    std::chrono::system_clock::time_point start = std::chrono::system_clock::now();

    std::vector<std::string> steps;

    steps.emplace_back("READY_TO_START");
    steps.emplace_back("ENTITY_GRID");
    waitForNeighbours(steps);
    steps.clear();


    steps.clear();

    this->step = "ENTITY_GRID";
    steps.emplace_back("ENTITY_GRID");
    steps.emplace_back("ENERGY_GIVEAWAY");
    waitForNeighbours(steps);
    steps.clear();

    processImportedAndExportedLifes();

    int deltaX = chunkPosition.getX() * TILE_PER_CHUNK;
    int deltaY = chunkPosition.getY() * TILE_PER_CHUNK;

    for (int it = 0; it < TILE_PER_CHUNK; it++) {
        for (int jt = 0; jt < TILE_PER_CHUNK; jt++) {
            entityGrid.at(it).at(jt).clear();
        }
    }

    for (int it = 0; it < lifes.size(); it++) {
        if (lifes.at(it)->getEntity() == nullptr) {
            std::cout << "Generating entity grid but life have no entity" << std::endl;
            continue;
        }

        Point position = lifes.at(it)->getEntity()->getPosition();
        Point tileCoordinate = position.getTileCoordinates();
        Point chunkCoordinates = position.getSimpleCoordinates();

        if (!checkForLifeTransfer(lifes.at(it))) {
            entityGrid.at(tileCoordinate.getX() - deltaX).at(tileCoordinate.getY() - deltaY).push_back(lifes.at(it)->getEntity());
        }
    }

    for (int it = 0; it < entities.size(); it++) {
        Point position = entities.at(it)->getPosition();
        Point tileCoordinate = position.getTileCoordinates();
        Point chunkCoordinates = position.getSimpleCoordinates();

        entityGrid.at(tileCoordinate.getX() - deltaX).at(tileCoordinate.getY() - deltaY).push_back(entities.at(it));
    }
}

void Chunk::handleEnergyGiveaway() {

    std::vector<std::string> steps;
    steps.clear();

    this->step = "ENERGY_GIVEAWAY";
    steps.emplace_back("ENERGY_GIVEAWAY");
    steps.emplace_back("CLIMATE_START");
    waitForNeighbours(steps);
    steps.clear();


    for (int it = 0; it < lifes.size(); it++) {
        Life *currentLife = lifes.at(it);
        Point entityPoint = currentLife->getEntity()->getPosition();
        Point tilePoint = entityPoint.getTileCoordinates();

        double releasedHeat = currentLife->giveawayEnergy();

        if (currentLife->getType() == "ANIMAL") {
            getTileAt(tilePoint.getX(), tilePoint.getY())->addHeat(releasedHeat);
        } else {
            getTileAt(tilePoint.getX(), tilePoint.getY())->addGround(releasedHeat);
        }

        if (currentLife->getEnergyManagement()->getEnergy() <= 0) {
            this->lifesToDelete.emplace_back(currentLife);
        }

    }

    removeDeletedEntities();

}

void Chunk::processClimate() {


    double newGround[TILE_PER_CHUNK][TILE_PER_CHUNK];
    double newHeats[TILE_PER_CHUNK][TILE_PER_CHUNK];

    this->step = "CLIMATE_START";
    std::vector<std::string> steps;
    steps.emplace_back("CLIMATE_START");
    steps.emplace_back("BRAIN_PROCESSING");
    waitForNeighbours(steps);
    steps.clear();

    for (int it = 0; it < TILE_PER_CHUNK; it++) {
        for (int jt = 0; jt < TILE_PER_CHUNK; jt++) {
            Tile * currentTile = getRelativeTile(it, jt, false);
            currentTile->processAddedGround();
            currentTile->processAddedHeat();

            newGround[it][jt] = currentTile->getGround();
            newHeats[it][jt] = currentTile->getHeat();
        }
    }

    int deltaX = chunkPosition.getX() * TILE_PER_CHUNK;
    int deltaY = chunkPosition.getY() * TILE_PER_CHUNK;

//    this->step = "CLIMATE_READY";
//    steps.clear();
//    steps.emplace_back("CLIMATE_READY");
//    steps.emplace_back("CLIMATE_SPREAD");
//    waitForNeighbours(steps);
//    steps.clear();

    for (int it = 0; it < TILE_PER_CHUNK; it++) {
        for (int jt = 0; jt < TILE_PER_CHUNK; jt++) {
            Tile * currentTile = tiles.at(it).at(jt);
//            std::cout << "Processing climate on tile X: " << currentTile->getPosition().getX() << " Y: " << currentTile->getPosition().getY() << std::endl;

            double availableGround = currentTile->getGround() / 100.0;


            double currentHeight = currentTile->getHeight();
            double availableHeat = currentTile->getHeat() / 10.0;

            if (availableGround == 0 && availableHeat == 0) {
                continue;
            }

            for (int x = -1; x <= 1; x++) {
                for (int y = -1; y <= 1; y++) {
                    int totalX = it + x + deltaX;
                    int totalY = jt + y + deltaY;
                    if (it + x + deltaX < 0 || it + x + deltaX >= TILE_COUNT_WIDTH ||jt + y + deltaY < 0 || jt + y + deltaY >= TILE_COUNT_HEIGHT) {
                        continue;
                    }

                    Tile * relativeTile = getRelativeTile(it + x, jt + y, false);
//
//                    if (relativeTile->getPosition().equals(currentTile->getPosition())) {
//                        continue;
//                    }


                    // If the other tile is lower than this one, we add some heat to be transfer
                    // the lowest the target tile, the biggest is the added energy
                    double heightDifference = (currentHeight - relativeTile->getHeight()) / 5.f;

                    double transferedHeat = availableHeat + (heightDifference * availableHeat);
                    relativeTile->addHeat(transferedHeat);
                    newHeats[it][jt] -= transferedHeat;




                    double transferedGround = availableGround + (heightDifference * availableGround);
                    relativeTile->addGround(transferedGround);
                    newGround[it][jt] -= transferedGround;
                }
            }

        }
    }



//    this->step = "CLIMATE_SPREAD";
//    steps.clear();
//    steps.emplace_back("CLIMATE_SPREAD");
//    steps.emplace_back("BRAIN_PROCESSING");
//    waitForNeighbours(steps);
//    steps.clear();


//    for (int it = 0; it < TILE_PER_CHUNK; it++) {
//        for (int jt = 0; jt < TILE_PER_CHUNK; jt++) {
//            Tile * currentTile = getRelativeTile(it, jt, false);
//            currentTile->setGround(newGround[it][jt]);
//            currentTile->setHeat(newHeats[it][jt]);
//            currentTile->processAddedGround();
//            currentTile->processAddedHeat();
//        }
//    }




    for (int it = 0; it < TILE_PER_CHUNK; it++) {
        for (int jt = 0; jt < TILE_PER_CHUNK; jt++) {
            Tile * currentTile = getRelativeTile(it, jt, false);

            if (newGround[it][jt] < 0) {
                std::cout << "Climate to ground " << newGround[it][jt] << std::endl;
            }
            currentTile->setGround(newGround[it][jt]);
            currentTile->setHeat(newHeats[it][jt]);

            currentTile->processAddedHeat();
            currentTile->processAddedGround();

            if (currentTile->getHeat() <= 0) {
                continue;
            }

            float heatToGroundRatio = 0.01f;
            float currentTileHeat = currentTile->getHeat();

            currentTile->setHeat(currentTile->getHeat() - (currentTileHeat * heatToGroundRatio));
            currentTile->addGround(currentTileHeat * heatToGroundRatio);

            currentTile->processAddedGround();
        }
    }


}

void Chunk::brainProcessing() {
    steps.clear();

    this->step = "BRAIN_PROCESSING";
    steps.emplace_back("BRAIN_PROCESSING");
    steps.emplace_back("EXECUTE_ACTIONS");
    waitForNeighbours(steps);
    steps.clear();
    processGlobalAddedEnergy();


    for (int it = 0; it < lifes.size(); it++) {
        Life *currentLife = lifes.at(it);

        currentLife->processSelectedTiles();

        std::vector<Entity *> accessibleEntities = getAccessibleEntities(currentLife->getSelectedTiles());
        std::vector<Tile *> accessibleTiles = getAccessibleTiles(currentLife->getSelectedTiles());

        currentLife->processSensors(accessibleEntities, accessibleTiles);


        currentLife->processBrain();

        std::vector<ActionDTO> currentCreatureActions = currentLife->executeExternalActions(accessibleEntities);
        actions.insert(actions.end(), currentCreatureActions.begin(), currentCreatureActions.end());

    }
}

void Chunk::executeCreaturesActions() {

    steps.clear();

    this->step = "EXECUTE_ACTIONS";
    steps.emplace_back("EXECUTE_ACTIONS");
    steps.emplace_back("MOVE_CREATURES");
    waitForNeighbours(steps);
    steps.clear();
    processGlobalAddedEnergy();


    int captureGroundActions = 0;
    int captureHeatActions = 0;
    int duplicateActions = 0;
    int mateActions = 0;
    int eatActions = 0;

    int naturalMatingCount = 0;
    for (int it = 0; it < actions.size(); it++) {
        ActionDTO actionDto = actions.at(it);


        Life * performer = getChunkLifeFromId(actionDto.getPerformerId());
        Entity * subject = getEntityFromId(actionDto.getSubjectId(), true);

        if (actionDto.getSubjectId() != 0 && subject == nullptr) {
            Life * subjectLife = getLifeFromId(actionDto.getSubjectId(), true);
            if (subjectLife == nullptr) {
                std::cout << "ERROR: Subject not found in entities nor in lifes" << std::endl;
                continue;
            }

            subject = subjectLife->getEntity();
        }


        if (!performer->getEnergyManagement()->isAlive()) {
            continue;
        }

        if (actionDto.getSubjectId() != 0 && (subject == nullptr || (subject != nullptr && !subject->isExists()))) {
            continue;
        }

        if (actionDto.getType() == "EAT") {
            float wastedEnergy = performer->addEnergy(subject->getMass());
            subject->setMass(0.0);

            Point performerPosition = performer->getEntity()->getPosition();
            Point tilePosition = performerPosition.getTileCoordinates();

            Life * foundLife = getLifeFromId(actionDto.getSubjectId(), true);
            if (foundLife != nullptr) {
                getTileAt(tilePosition.getX(), tilePosition.getY())->addHeat(foundLife->getEnergyManagement()->getEnergy());
                lifesToDelete.emplace_back(foundLife);
            }

            getTileAt(tilePosition.getX(), tilePosition.getY())->addHeat(wastedEnergy);


            Point performerPoint = performer->getEntity()->getPosition();
            Point tilePoint = performerPoint.getTileCoordinates();

            entityToDelete.emplace_back(subject);
            eatActions++;
        }

        if (actionDto.getType() == "MATE") {
            bool success = handleMating(performer, subject->getId());

            if (success) {
                naturalMatingCount++;
                mateActions++;
            }

        }

        if (actionDto.getType() == "DUPLICATE") {
            bool success = handleDuplication(performer);

            if (success) {
                naturalMatingCount++;
                duplicateActions++;
            }

        }

        if (actionDto.getType() == "CAPTURE_GROUND") {
            handleCaptureGround(performer, actionDto);
            captureGroundActions++;

        }

        if (actionDto.getType() == "CAPTURE_HEAT") {
            handleCaptureHeat(performer, actionDto);
            captureHeatActions++;
        }

    }

    removeDeletedEntities();
    actions.clear();

    int totalActions = captureGroundActions + captureHeatActions + duplicateActions + mateActions + eatActions;

    dataAnalyser->getTotalActions()->addRawToTick(tick, totalActions);
    dataAnalyser->getCaptureGroundActions()->addRawToTick(tick, captureGroundActions);
    dataAnalyser->getCaptureHeatActions()->addRawToTick(tick, captureHeatActions);
    dataAnalyser->getDuplicateActions()->addRawToTick(tick, duplicateActions);
    dataAnalyser->getMateActions()->addRawToTick(tick, mateActions);
    dataAnalyser->getEatActions()->addRawToTick(tick, eatActions);

}

void Chunk::moveCreatures() {

    steps.clear();

    this->step = "MOVE_CREATURES";
    steps.emplace_back("MOVE_CREATURES");
    steps.emplace_back("STATISTICS");
    waitForNeighbours(steps);
    steps.clear();


    std::vector<Life *> exportedLifes;
    for (int it = 0; it < lifes.size(); it++) {
        Life *currentLife = lifes.at(it);
        Point entityPoint = currentLife->getEntity()->getPosition();
        Point tilePoint = entityPoint.getTileCoordinates();

        std::vector<Entity* > producedEntities = currentLife->executeInternalActions();
//        newEntities.insert(newEntities.begin(), producedEntities.begin(), producedEntities.end());

        checkForLifeTransfer(currentLife);
    }





    removeDeletedEntities();

}

void Chunk::statistics() {

    steps.clear();

    this->step = "STATISTICS";
    steps.emplace_back("STATISTICS");
    steps.emplace_back("TICK_PASS");
    waitForNeighbours(steps);
    steps.clear();


    std::chrono::system_clock::time_point start = std::chrono::system_clock::now();



//    std::vector<Life *> sortedLife = getScoreSortedCreatures();
//    std::vector<BrainConnector *> sortedConnectors = connectors;
    int populationSize = lifes.size();

    dataAnalyser->getPopulation()->addRawToTick(tick, populationSize);

    int totalAnimals = 0;
    int totalVegetals = 0;

    double totalPopulationScore = 0.0;
    for (int it = 0; it < populationSize; it++) {
        totalPopulationScore += lifes.at(it)->getEntity()->getAge();
        if (lifes.at(it)->getType() == "ANIMAL") {
            totalAnimals++;
        } else {
            totalVegetals++;
        }
    }

    dataAnalyser->getAnimals()->addRawToTick(tick, totalAnimals);
    dataAnalyser->getVegetals()->addRawToTick(tick, totalVegetals);

//    double averagePopulationAge = totalPopulationScore / double(populationSize);
//
//    double maxScore = lifes.at(0)->getEntity()->getAge();
//
//    double firstQuartileScore = lifes.at(populationSize / 4)->getEntity()->getAge();
//    double median = lifes.at(populationSize / 2)->getEntity()->getAge();
//    double lastQuartileScore = lifes.at((3 * populationSize) / 4)->getEntity()->getAge();
//
//
//    dataAnalyser->getAverageScore()->addRawToTick(tick, averagePopulationAge);
//    dataAnalyser->getBestScore()->addRawToTick(tick, maxScore);
//    dataAnalyser->getFirstQuartileScore()->addRawToTick(tick, firstQuartileScore);
//    dataAnalyser->getMedianScore()->addRawToTick(tick, median);
//    dataAnalyser->getLastQuartileScore()->addRawToTick(tick, lastQuartileScore);




    double totalCreaturesEnergy = 0.f;
    double totalCreaturesMass = 0.f;
    double totalFoodsMass = 0.f;


    for (int it = 0; it < lifes.size(); it++) {
        Life * currentLife = lifes.at(it);
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

    for (int it = 0; it < TILE_PER_CHUNK; it++) {
        for (int jt = 0; jt < TILE_PER_CHUNK; jt++) {
            Tile * tile = getRelativeTile(it, jt, false);
            totalHeat += tile->getHeat();
            totalGround += tile->getGround();
            totalToAdd += tile->getAddedHeat();
            totalToAdd += tile->getAddedGround();
        }
    }


    int totalEnergy = totalFoodsMass + totalCreaturesMass + totalCreaturesEnergy + totalHeat + totalGround + totalToAdd;

//    std::cout << "Tick: " << tickCount << " Total: " << totalEnergy << " Difference: " << totalEnergy - dataAnalyser->getTotalEnergy()->getLastValue() << std::endl;

    dataAnalyser->getTotalEnergy()->addRawToTick(tick, totalEnergy);
    dataAnalyser->getFoodEnergy()->addRawToTick(tick, totalFoodsMass);
    dataAnalyser->getCreaturesMass()->addRawToTick(tick, totalCreaturesMass);
    dataAnalyser->getCreaturesEnergy()->addRawToTick(tick, totalCreaturesEnergy);
    dataAnalyser->getHeatEnergy()->addRawToTick(tick, totalHeat);
    dataAnalyser->getGroundEnergy()->addRawToTick(tick, totalGround);
    dataAnalyser->getEnergyToAdd()->addRawToTick(tick, totalToAdd);




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

    dataAnalyser->getStatisticsTime()->addRawToTick(tick, statisticsTime);
    totalTime += statisticsTime;

    dataAnalyser->getTotalTime()->setRawToTick(tick, totalTime);

    if (chunkPosition.equals(Point(0, 0))/* && tick % 100 == 0*/) {
        populationControl();
    }

}

void Chunk::aTickHavePassed() {
    steps.clear();

    this->step = "TICK_PASS";
    steps.emplace_back("TICK_PASS");
    steps.emplace_back("READY_TO_START");
    waitForNeighbours(steps);
    steps.clear();


    for (int it = 0; it < lifes.size(); it++) {
        lifes.at(it)->getEntity()->aTickHavePassed();
    }
    for (int it = 0; it < entities.size(); it++) {
        entities.at(it)->aTickHavePassed();
    }
    tick++;

    this->step = "READY_TO_START";


}

void Chunk::populationControl() {

    int currentAnimalCount = dataAnalyser->getAnimals()->getThirdToLastValue();
    if (currentAnimalCount > int(INITIAL_CREATURE_COUNT / 2) - (INITIAL_CREATURE_COUNT * 0.05)) {
        return;
    }


    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> distWidth(0, FARM_WIDTH);
    std::uniform_real_distribution<double> distHeight(0, FARM_HEIGHT);

    std::vector<Point> *empty = new std::vector<Point>();
    std::vector<Life *> allLifes = getAllLifes(empty);

    std::vector<Life *> creatures;
    for (int it = 0; it < allLifes.size(); it++) {
        if (allLifes.at(it)->getType() == "ANIMAL") {
            creatures.emplace_back(allLifes.at(it));
        }
    }


    if (creatures.size() > int(INITIAL_CREATURE_COUNT / 2) - (INITIAL_CREATURE_COUNT * 0.05)) {
        return;
    }

    std::cout << "Population control" << std::endl;



    int selectedParentCount = creatures.size();

    int newConnectorNeeded = int(INITIAL_CREATURE_COUNT / 2) + (INITIAL_CREATURE_COUNT * 0.05) - creatures.size();

    float totalEnergyRemoved = 0.f;
    for (int it = 0; it < newConnectorNeeded; it++) {
        int fatherIndex = rand() % selectedParentCount;
        int motherIndex = rand() % selectedParentCount;

        Life * father = creatures.at(fatherIndex);
        Life * mother = creatures.at(motherIndex);

        Life * child = this->nursery->Mate(father, mother);
        child->getEnergyManagement()->setEnergy(child->getEnergyManagement()->getMaxMass() / 2.f);
        child->setMass(child->getEnergyManagement()->getMaxMass() / 2.f);
        Entity * childCreature = child->getEntity();

//        float childSpawnX = distWidth(mt);
//        float childSpawnY = distHeight(mt);
//        Point childCreaturePosition = Point(childSpawnX, childSpawnY);
//
//        childCreature->setPosition(childCreaturePosition);

        totalEnergyRemoved += child->getEntity()->getMass() + child->getEnergyManagement()->getEnergy();

        Point entityPoint = childCreature->getPosition();
        Point entityChunkPoint = entityPoint.getSimpleCoordinates();

        addLife(child);
    }

    empty->clear();
    std::vector<Tile *> allTiles = getAllTiles(empty);

    double totalGround = 0.0;
    for (int it = 0; it < allTiles.size(); it++) {
        Tile * currentTile = allTiles.at(it);
        currentTile->lockGroundMutex();

        totalGround += currentTile->getGround();
    }

    if (totalEnergyRemoved > totalGround) {
        std::cout << "Error, not enough energy" << std::endl;
    }

    double totalEnergyRemovedFromGround = 0.0;
    for (int it = 0; it < allTiles.size(); it++) {
        Tile * currentTile = allTiles.at(it);
        double ratio = currentTile->getGround() / totalGround;

        currentTile->lockOwnerRemoveGround(ratio * totalEnergyRemoved);
        currentTile->unlockGroundMutex();

        totalEnergyRemovedFromGround += ratio * totalEnergyRemoved;
    }

//    double totalGroundAfter = 0.0;
//    for (int it = 0; it < allTiles.size(); it++) {
//        Tile * currentTile = allTiles.at(it);
//        currentTile->lockGroundMutex();
//
//        totalGroundAfter += currentTile->getGround();
//    }
//
//    std::cout << "Before: " << totalGround << " After: " << totalGroundAfter << " Difference: " << (totalGround - totalEnergyRemoved) - totalGroundAfter  << std::endl;


        // TODO
//    map->removeEnergyFromGround(totalEnergyRemoved);



}


void Chunk::removeEnergy(Point targetChunk, double energyToRemove, std::vector<Point> *visitedPoints) {
    for (int it = 0; it < visitedPoints->size(); it++) {
        if (chunkPosition.equals(visitedPoints->at(it))) {
            return;
        }
    }

    visitedPoints->emplace_back(chunkPosition);


    if (chunkPosition.equals(targetChunk)) {

        double totalGround = 0.0;

        for (int it = 0; it < TILE_PER_CHUNK; it++) {
            for (int jt = 0; jt < TILE_PER_CHUNK; jt++) {
                Tile * currentTile = getRelativeTile(it, jt, false);
                currentTile->lockGroundMutex();

                totalGround += currentTile->getGround();
            }
        }


        if (energyToRemove > totalGround) {
            for (int it = 0; it < TILE_PER_CHUNK; it++) {
                for (int jt = 0; jt < TILE_PER_CHUNK; jt++) {
                    Tile * currentTile = getRelativeTile(it, jt, false);
                    currentTile->unlockGroundMutex();
                }
            }

            return;
        }


        double removedEnergy = 0.0;
        for (int it = 0; it < TILE_PER_CHUNK; it++) {
            for (int jt = 0; jt < TILE_PER_CHUNK; jt++) {
                Tile * tile = getRelativeTile(it, jt, false);
                double ratio = tile->getGround() / totalGround;

                tile->lockOwnerRemoveGround(ratio * energyToRemove);
                tile->unlockGroundMutex();
                removedEnergy += ratio * energyToRemove;
            }
        }

        return;
    }



    visitedPoints->emplace_back(chunkPosition);

    for (int it = 0; it < 3; it++) {
        for (int jt = 0; jt < 3; jt++) {

            Chunk * neighbour = neighbours.at(it).at(jt);

            if (neighbour == nullptr || neighbour->getChunkPosition().equals(chunkPosition))
                continue;

            neighbour->removeEnergy(targetChunk, energyToRemove, visitedPoints);
        }
    }

}

void Chunk::processGlobalAddedEnergy() {
    for (int it = 0; it < TILE_PER_CHUNK; it++) {
        for (int jt = 0; jt < TILE_PER_CHUNK; jt++) {
            Tile * currentTile = getRelativeTile(it, jt, false);
            currentTile->processAddedGround();
            currentTile->processAddedHeat();
        }
    }
}


// Find entities and lifes
Tile * Chunk::getRelativeTile(int tileX, int tileY, bool debug) {
    int deltaX = chunkPosition.getX() * TILE_PER_CHUNK;
    int deltaY = chunkPosition.getY() * TILE_PER_CHUNK;

//    if (debug) {
//        std::cout << "Requested tile X: " << tileX + deltaX << " Y: " << tileY + deltaY;
//    }

    if (tileX < 0 || tileX >= TILE_PER_CHUNK || tileY < 0 || tileY >= TILE_PER_CHUNK) {
        int ratioX = 0;
        if (tileX < 0)
            ratioX = -1;
        if (tileX >= TILE_PER_CHUNK)
            ratioX = 1;

        int ratioY = 0;
        if (tileY < 0)
            ratioY = -1;
        if (tileY >= TILE_PER_CHUNK)
            ratioY = 1;



        int requestedX = deltaX + tileX;
        int requestedY = deltaY + tileY;

        Tile * foundTile = neighbours.at(ratioX + 1).at(ratioY + 1)->getTileAt(requestedX, requestedY);

//        if (debug) {
//            std::cout << " => X: " << foundTile->getPosition().getX() << " Y: " << foundTile->getPosition().getY() << std::endl;
//        }


        return foundTile;
    }

    Tile * tile = tiles.at(tileX).at(tileY);

//    if (debug) {
//        std::cout << " => X: " << tile->getPosition().getX() << " Y: " << tile->getPosition().getY() << std::endl;
//    }
    return tile;
}

Tile * Chunk::getTileAt(int tileX, int tileY) {
    if (tileX < 0 || tileX >= TILE_COUNT_WIDTH || tileY < 0 || tileY >= TILE_COUNT_HEIGHT) {
        std::cout << "ERROR, REQUESTED WRONG TILE => X: " << tileX << " Y: " << tileY << std::endl;
        return tiles.at(0).at(0);
    }

    Point requestedChunkPosition = Point(tileX / TILE_PER_CHUNK, tileY / TILE_PER_CHUNK);

    if (!requestedChunkPosition.equals(chunkPosition)) {
        std::cout << "ERROR, REQUESTED TILE IN WRONG CHUNK: X: " << tileX << " Y: " << tileY << " | ";
        std::cout << " CHUNK: X: " << tileX / TILE_PER_CHUNK << " Y: " << tileY / TILE_PER_CHUNK << " | ";
        std::cout << "CURRENT CHUNK => X: " << chunkPosition.getX() << " Y: " << chunkPosition.getY() << std::endl;
        return tiles.at(0).at(0);
    }

    int deltaX = chunkPosition.getX() * TILE_PER_CHUNK;
    int deltaY = chunkPosition.getY() * TILE_PER_CHUNK;

    Tile * tile = tiles.at(tileX - deltaX).at(tileY - deltaY);

    return tile;
}

std::vector<Entity *> Chunk::getRelativeEntities(int tileX, int tileY) {

    if (tileX < 0 || tileX >= TILE_PER_CHUNK || tileY < 0 || tileY >= TILE_PER_CHUNK) {
        int ratioX = 0;
        if (tileX < 0)
            ratioX = -1;
        if (tileX >= TILE_PER_CHUNK)
            ratioX = 1;

        int ratioY = 0;
        if (tileY < 0)
            ratioY = -1;
        if (tileY >= TILE_PER_CHUNK)
            ratioY = 1;

        int deltaX = chunkPosition.getX() * TILE_PER_CHUNK;
        int deltaY = chunkPosition.getY() * TILE_PER_CHUNK;

        int requestedX = deltaX + tileX;
        int requestedY = deltaY + tileY;

        return neighbours.at(ratioX + 1).at(ratioY + 1)->getEntitiesAt(requestedX, requestedY);
    }


    return entityGrid.at(tileX).at(tileY);
}

std::vector<Entity *> Chunk::getEntitiesAt(int tileX, int tileY) {
    if (tileX < 0 || tileX >= TILE_COUNT_WIDTH || tileY < 0 || tileY >= TILE_COUNT_HEIGHT) {
        std::cout << "ERROR, REQUESTED WRONG ENTITIES => X: " << tileX << " Y: " << tileY << std::endl;
        return entityGrid.at(0).at(0);
    }

    Point requestedChunkPosition = Point(tileX / TILE_PER_CHUNK, tileY / TILE_PER_CHUNK);

    if (!requestedChunkPosition.equals(chunkPosition)) {
        std::cout << "ERROR, REQUESTED ENTITIES IN WRONG CHUNK: X: " << tileX << " Y: " << tileY << " | ";
        std::cout << "CURRENT CHUNK => X: " << chunkPosition.getX() << " Y: " << chunkPosition.getY() << std::endl;
        return entityGrid.at(0).at(0);
    }

    int deltaX = chunkPosition.getX() * TILE_PER_CHUNK;
    int deltaY = chunkPosition.getY() * TILE_PER_CHUNK;

    return entityGrid.at(tileX - deltaX).at(tileY - deltaY);
}

Life * Chunk::getLifeFromId(int id, bool askNeighbours) {

    std::vector<Life *> currentLifes = this->lifes;
    for (int it = 0; it < currentLifes.size(); it++) {
        if (currentLifes.at(it)->getEntity()->getId() == id) {
            return currentLifes.at(it);
        }
    }

    if (askNeighbours == false) {
        return nullptr;
    }

    Life * found = nullptr;

    for (int it = 0; it < 3; it++) {
        for (int jt = 0; jt < 3; jt++) {

            Chunk * neighbour = neighbours.at(it).at(jt);

            if (neighbour == nullptr || neighbour->getChunkPosition().equals(chunkPosition))
                continue;

            Life * neighbourResponse = neighbour->getLifeFromId(id, false);

            if (neighbourResponse != nullptr) {
                found = neighbourResponse;
            }

        }
    }

    return found;
}

Life * Chunk::getChunkLifeFromId(int id) {

    std::vector<Life *> currentLifes;
    currentLifes.insert(currentLifes.end(), lifes.begin(), lifes.end());
    for (int it = 0; it < currentLifes.size(); it++) {
        if (currentLifes.at(it)->getEntity()->getId() == id) {
            return currentLifes.at(it);
        }
    }

    return nullptr;
}

Entity * Chunk::getEntityFromId(int id, bool askNeighbours) {
    for (int it = 0; it < this->entities.size(); it++) {
        if (this->entities.at(it)->getId() == id) {
            return this->entities.at(it);
        }
    }

    if (!askNeighbours)
        return nullptr;

    Entity * found = nullptr;

    for (int it = 0; it < 3; it++) {
        for (int jt = 0; jt < 3; jt++) {

            Chunk * neighbour = neighbours.at(it).at(jt);

            if (neighbour == nullptr || neighbour->getChunkPosition().equals(chunkPosition))
                continue;

            Entity * neighbourResponse = neighbour->getEntityFromId(id, false);

            if (neighbourResponse != nullptr) {
                found = neighbourResponse;
            }

        }
    }



    return found;
}

std::vector<Entity *> Chunk::getAccessibleEntities(std::vector<Point> selectedTiles) {
    int deltaX = chunkPosition.getX() * TILE_PER_CHUNK;
    int deltaY = chunkPosition.getY() * TILE_PER_CHUNK;

    std::vector<Entity *> accessibleEntities;
    for (int jt = 0; jt < selectedTiles.size(); jt++) {
        Point currentTiles = selectedTiles.at(jt);

        std::vector<Entity *> chunkEntities = getRelativeEntities(currentTiles.getX() - deltaX, currentTiles.getY() - deltaY);

        accessibleEntities.insert(accessibleEntities.end(), chunkEntities.begin(), chunkEntities.end());
    }
    return accessibleEntities;

}

std::vector<Tile *> Chunk::getAccessibleTiles(std::vector<Point> selectedTiles) {
    int deltaX = chunkPosition.getX() * TILE_PER_CHUNK;
    int deltaY = chunkPosition.getY() * TILE_PER_CHUNK;

    std::vector<Tile *> accessibleTiles;
    for (int jt = 0; jt < selectedTiles.size(); jt++) {
        Point currentTilePoint = selectedTiles.at(jt);

        Tile * currentTile = getRelativeTile(currentTilePoint.getX() - deltaX, currentTilePoint.getY() - deltaY, false);

        accessibleTiles.emplace_back(currentTile);
    }
    return accessibleTiles;

}

std::vector<Tile *> Chunk::getAllTiles(std::vector<Point> *visitedPoints) {
    std::vector<Tile *> foundTiles;

    for (int it = 0; it < visitedPoints->size(); it++) {
        if (chunkPosition.equals(visitedPoints->at(it))) {
            return foundTiles;
        }
    }
    visitedPoints->emplace_back(chunkPosition);

    for (int it = 0; it < 3; it++) {
        for (int jt = 0; jt < 3; jt++) {

            Chunk * neighbour = neighbours.at(it).at(jt);

            if (neighbour == nullptr || neighbour->getChunkPosition().equals(chunkPosition))
                continue;

            std::vector<Tile *>  neighbourResponse = neighbour->getAllTiles(visitedPoints);
            if (neighbourResponse.size() > 0) {
                foundTiles.insert(foundTiles.end(), neighbourResponse.begin(), neighbourResponse.end());
            }
        }
    }



    for (int it = 0; it < TILE_PER_CHUNK; it++) {
        for (int jt = 0; jt < TILE_PER_CHUNK; jt++) {
            foundTiles.emplace_back(tiles.at(it).at(jt));
        }
    }

    return foundTiles;
}

std::vector<Life *> Chunk::getAllLifes(std::vector<Point> *visitedPoints) {
    std::vector<Life *> foundLifes;


    for (int it = 0; it < visitedPoints->size(); it++) {
        if (chunkPosition.equals(visitedPoints->at(it))) {
            return foundLifes;
        }
    }
    visitedPoints->emplace_back(chunkPosition);

    for (int it = 0; it < 3; it++) {
        for (int jt = 0; jt < 3; jt++) {

            Chunk * neighbour = neighbours.at(it).at(jt);

            if (neighbour == nullptr || neighbour->getChunkPosition().equals(chunkPosition))
                continue;

            std::vector<Life *>  neighbourResponse = neighbour->getAllLifes(visitedPoints);
            if (neighbourResponse.size() > 0) {
                foundLifes.insert(foundLifes.end(), neighbourResponse.begin(), neighbourResponse.end());
            }
        }
    }

    std::vector<Life *> currentLifes;
    currentLifes.insert(currentLifes.end(), lifes.begin(), lifes.end());
    for (int it = 0; it < currentLifes.size(); it++) {
        foundLifes.emplace_back(currentLifes.at(it));
    }

    return foundLifes;
}




// The neighbourhood
void Chunk::generateNeighbours() {
    for (int it = -1; it <= 1; it++) {
        std::vector<Chunk *> neighbourgsLine;
        for (int jt = -1; jt <= 1; jt++) {
            neighbourgsLine.emplace_back(nullptr);
        }
        neighbours.emplace_back(neighbourgsLine);
    }
}

void Chunk::generateRandomChunk(int seed, float min, float max) {
    PerlinNoise perlin(seed);

    float removed = ((max - min) / 2.f) + min;
    float ratio = (1.f / (max - min)) * 2.f;

    int deltaX = chunkPosition.getX() * TILE_PER_CHUNK;
    int deltaY = chunkPosition.getY() * TILE_PER_CHUNK;

    for (int it = 0; it < TILE_PER_CHUNK; it++) {
        std::vector<Tile *> line;
        for (int jt = 0; jt < TILE_PER_CHUNK; jt++) {

            float xComponent = (float(it + deltaX) / float(TILE_COUNT_WIDTH)) * 2.5;
            float yComponent = (float(jt + deltaY) / float(TILE_COUNT_HEIGHT)) * 2.5;

            float height = perlin.noise(xComponent, yComponent, 0.8);

            height -= removed;
            height *= ratio;
            height *= height * height;

            Tile * currentTile = new Tile(Point(it + deltaX, jt + deltaY));
            currentTile->setHeight(height);
            currentTile->setGround(1500.0);

            line.emplace_back(currentTile);

            currentTile->setHeight(height);

        }
        tiles.emplace_back(line);
    }
}

void Chunk::setNeighbour(int it, int jt, Chunk * neighbour) {
    neighbours.at(it + 1).at(jt + 1) = neighbour;
}

Chunk *Chunk::getNeighbourAt(int it, int jt) {
    return neighbours.at(it).at(jt);
}

void Chunk::waitForNeighbours(std::vector<std::string> requestedSteps) {
    steps.clear();

    bool allNeighboursReady(true);

    int count = 1;
    do {
        allNeighboursReady = true;

        for (int it = 0; it < 3; it++) {
            for (int jt = 0; jt < 3; jt++) {

                Chunk * neighbour = neighbours.at(it).at(jt);

                if (neighbour == nullptr || neighbour->getChunkPosition().equals(chunkPosition))
                    continue;

                bool found(false);
                for (int kt = 0; kt < requestedSteps.size(); kt++) {
                    if (neighbour->getStep() == requestedSteps.at(kt)) {
                        found = true;
                    }
                }

                if (!found) {
                    allNeighboursReady = false;
                }

            }
        }

        if (!allNeighboursReady) {
            usleep(count * 1000);
            count++;
        }

    }while (!allNeighboursReady);
}

void Chunk::getNeighboursReady(std::vector<Point> *visitedPoints) {

    for (int it = 0; it < visitedPoints->size(); it++) {
        if (chunkPosition.equals(visitedPoints->at(it))) {
            return;
        }
    }
    visitedPoints->emplace_back(chunkPosition);


    for (int it = 0; it < 3; it++) {
        for (int jt = 0; jt < 3; jt++) {
            Chunk * neighbour = neighbours.at(it).at(jt);

            if (neighbour == nullptr || neighbour->getChunkPosition().equals(chunkPosition))
                continue;

            neighbour->getNeighboursReady(visitedPoints);
        }
    }

    this->step = "READY_TO_START";
}



// In and Out
void Chunk::removeDeletedEntities() {
    std::lock_guard<std::mutex> guard(entity_changes_mutex);

    std::vector<Life *> newLifes;
    for (int it = 0; it < lifes.size(); it++) {
        Life * currentLife = lifes.at(it);
        if (currentLife->getEnergyManagement()->getEnergy() > 0 && currentLife->getEntity()->getMass() > 0) {
            newLifes.emplace_back(lifes.at(it));
        } else {
            Life * currentLife = lifes.at(it);
            Point currentLifePosition = currentLife->getEntity()->getPosition();
            Point currentLifeTilePosition = currentLifePosition.getTileCoordinates();

            getTileAt(int(currentLifeTilePosition.getX()), int(currentLifeTilePosition.getY()))->addGround(currentLife->getEntity()->getMass());
            getTileAt(int(currentLifeTilePosition.getX()), int(currentLifeTilePosition.getY()))->addHeat(currentLife->getEnergyManagement()->getEnergy());
            lifesToDelete.emplace_back(currentLife);
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

bool Chunk::checkForLifeTransfer(Life * life) {
    int deltaX = chunkPosition.getX() * TILE_PER_CHUNK;
    int deltaY = chunkPosition.getY() * TILE_PER_CHUNK;

    Point lifeSpawnPoint = life->getEntity()->getPosition();
    Point tileLifeSpawnPoint = lifeSpawnPoint.getTileCoordinates();

    int tileX = tileLifeSpawnPoint.getX() - deltaX;
    int tileY = tileLifeSpawnPoint.getY() - deltaY;

    if (!(tileX < 0 || tileX >= TILE_PER_CHUNK || tileY < 0 || tileY >= TILE_PER_CHUNK)) {
        return false;
    }

    int ratioX = 0;
    if (tileX < 0)
        ratioX = -1;
    if (tileX >= TILE_PER_CHUNK)
        ratioX = 1;

    int ratioY = 0;
    if (tileY < 0)
        ratioY = -1;
    if (tileY >= TILE_PER_CHUNK)
        ratioY = 1;

    int requestedX = deltaX + tileX;
    int requestedY = deltaY + tileY;

    neighbours.at(ratioX + 1).at(ratioY + 1)->transferLife(life);
    exportedLifes.emplace_back(life);
    return true;
}

void Chunk::transferLife(Life * life) {

    importedLifes.emplace_back(life);
}

bool Chunk::addLife(Life * life) {

    int counter = 0;
    while (!entity_changes_mutex.try_lock()) {

        usleep(10000);
        if (counter == 10) {
            return false;
        }
        counter++;
    }


    int deltaX = chunkPosition.getX() * TILE_PER_CHUNK;
    int deltaY = chunkPosition.getY() * TILE_PER_CHUNK;

    Point lifeSpawnPoint = life->getEntity()->getPosition();
    Point tileLifeSpawnPoint = lifeSpawnPoint.getTileCoordinates();

    int tileX = tileLifeSpawnPoint.getX() - deltaX;
    int tileY = tileLifeSpawnPoint.getY() - deltaY;

    if (tileX < 0 || tileX >= TILE_PER_CHUNK || tileY < 0 || tileY >= TILE_PER_CHUNK) {
        int ratioX = 0;
        if (tileX < 0)
            ratioX = -1;
        if (tileX >= TILE_PER_CHUNK)
            ratioX = 1;

        int ratioY = 0;
        if (tileY < 0)
            ratioY = -1;
        if (tileY >= TILE_PER_CHUNK)
            ratioY = 1;

        int requestedX = deltaX + tileX;
        int requestedY = deltaY + tileY;

        bool response = neighbours.at(ratioX + 1).at(ratioY + 1)->addLife(life);
        entity_changes_mutex.unlock();
        return response;
    }



    lifes.emplace_back(life);
    lifesAdded.emplace_back(life);

    entity_changes_mutex.unlock();
    return true;
}

void Chunk::processImportedAndExportedLifes() {
    std::lock_guard<std::mutex> guard(entity_changes_mutex);

    std::vector<Life *> newLifes;
    for (int it = 0; it < lifes.size(); it++) {

        bool found = false;
        int foundIndex = -1;
        for (int jt = 0; jt < exportedLifes.size(); jt++) {
            if (lifes.at(it)->getEntity()->getId() == exportedLifes.at(jt)->getEntity()->getId()) {
                found = true;
                foundIndex = jt;
            }
        }

        if (!found) {
            newLifes.emplace_back(lifes.at(it));
        }
    }

    lifes.clear();
    lifes = newLifes;


    lifes.insert(lifes.end(), importedLifes.begin(), importedLifes.end());

    importedLifes.clear();
    exportedLifes.clear();

}



// The actions
void Chunk::handleCaptureHeat(Life * life, ActionDTO action) {

}

void Chunk::handleCaptureGround(Life * life, ActionDTO action) {
    int chunkReach = life->getEntity()->getSize() / 5.0;

//    chunkReach = std::max(double(chunkReach), 1.0);


    double totalGround = 0.0;
    int ratioSize = (chunkReach * 2) + 1;
    double ratios[ratioSize * ratioSize];

    Point entityPoint = life->getEntity()->getPosition();
    Point tilePoint = entityPoint.getTileCoordinates();

    int deltaX = chunkPosition.getX() * TILE_PER_CHUNK;
    int deltaY = chunkPosition.getY() * TILE_PER_CHUNK;

    for (int it = -chunkReach; it <= chunkReach; it++) {
        for (int jt = -chunkReach; jt <= chunkReach; jt++) {
            int currentTileX = tilePoint.getX() + it;
            int currentTileY = tilePoint.getY() + jt;

            if (currentTileX < 0 || currentTileX >= TILE_COUNT_WIDTH || currentTileY < 0 || currentTileY >= TILE_COUNT_HEIGHT)
                continue;

            Tile * currentTile = getRelativeTile(currentTileX - deltaX, currentTileY - deltaY, true);

            currentTile->lockGroundMutex();

            totalGround += currentTile->getGround();
        }
    }

    for (int it = -chunkReach; it <= chunkReach; it++) {
        for (int jt = -chunkReach; jt <= chunkReach; jt++) {
            int currentTileX = tilePoint.getX() + it;
            int currentTileY = tilePoint.getY() + jt;

            if (currentTileX < 0 || currentTileX >= TILE_COUNT_WIDTH || currentTileY < 0 || currentTileY >= TILE_COUNT_HEIGHT)
                continue;

            Tile * tile = getRelativeTile(currentTileX - deltaX, currentTileY - deltaY, false);
            double ratio = tile->getGround() / totalGround;
            int currentIndex = ((it + chunkReach) * ratioSize) + (jt + chunkReach);
            ratios[currentIndex] = ratio;
        }
    }
////
    double totalAimedEnergy = action.getValue() * totalGround;
//
    double totalCollectedEnergy = 0;
    for (int it = -chunkReach; it <= chunkReach; it++) {
        for (int jt = -chunkReach; jt <= chunkReach; jt++) {
            int currentTileX = tilePoint.getX() + it;
            int currentTileY = tilePoint.getY() + jt;

            if (currentTileX < 0 || currentTileX >= TILE_COUNT_WIDTH || currentTileY < 0 || currentTileY >= TILE_COUNT_HEIGHT)
                continue;

            int currentIndex = ((it + chunkReach) * ratioSize) + (jt + chunkReach);

            double currentRatio = ratios[currentIndex] * 0.1;
            Tile * tile = getRelativeTile(currentTileX - deltaX, currentTileY - deltaY, false);

            double tileEnergy = tile->getGround();
            double tileCollectedEnergy = (currentRatio * totalAimedEnergy);

            if (tileEnergy - tileCollectedEnergy < 0) {
                std::cout << "Error while removing from ground: " << tileEnergy - tileCollectedEnergy << std::endl;
            }

            totalCollectedEnergy += tile->lockOwnerRemoveGround(tileCollectedEnergy);
            tile->unlockGroundMutex();
        }
    }



    double wastedEnergy = life->addEnergy(totalCollectedEnergy);

    getTileAt(tilePoint.getX(), tilePoint.getY())->addGround(wastedEnergy);
    getTileAt(tilePoint.getX(), tilePoint.getY())->processAddedGround();

}

bool Chunk::handleDuplication(Life * life) {

    bool fatherCanReproduce = life->getEntity()->getMass() > life->getEnergyManagement()->getMaxMass() * 0.75f;

    if (!fatherCanReproduce) {
//        std::cout << "Father cannot reproduce" << std::endl;
        return false;
    }
//
    Life * child = this->nursery->Mate(life, nullptr);

    double givenEnergyToChildGoal = child->getEnergyManagement()->getMaxMass() / 20.f;

    double givenFatherEnergy = std::min(life->getEnergyManagement()->getEnergy(), givenEnergyToChildGoal);

    double actualGivenFatherEnergy = life->getEnergyManagement()->removeEnergy(givenFatherEnergy);

    if (givenFatherEnergy != actualGivenFatherEnergy) {
        std::cout << "Wrong energy given" << std::endl;
    }

    double totalGivenEnergy = actualGivenFatherEnergy;

    if (totalGivenEnergy > givenEnergyToChildGoal / 20.0) {
        child->getEnergyManagement()->setEnergy(totalGivenEnergy / 2.0);
        child->setMass(totalGivenEnergy / 2.0);
        int counter = 0;
        while (!addLife(child)) {
            counter++;
            std::cout << "Unable to add life (duplication), waiting for " << counter << "ms" << std::endl;
            usleep(counter * 1000);
        }
        return true;
    }


    Point childCoordinate = child->getEntity()->getPosition();
    Point tileChildPosition = childCoordinate.getTileCoordinates();

    int deltaX = chunkPosition.getX() * TILE_PER_CHUNK;
    int deltaY = chunkPosition.getY() * TILE_PER_CHUNK;

    getRelativeTile(tileChildPosition.getX() - deltaX, tileChildPosition.getY() - deltaY, false)->addGround(totalGivenEnergy);


    if (life->getEnergyManagement()->getEnergy() <= 0) {
        std::lock_guard<std::mutex> guard(entity_changes_mutex);
        getRelativeTile(tileChildPosition.getX() - deltaX, tileChildPosition.getY() - deltaY, false)->addGround(life->getEntity()->getMass());
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

bool Chunk::handleMating(Life * father, int entityId) {
    Life * foundLife = getLifeFromId(entityId, true);
    if (foundLife == nullptr) {
        return false;
    }

    if (father->getType() != "ANIMAL" || foundLife->getType() != "ANIMAL") {
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

        int counter = 0;
        while (!addLife(child)) {
            counter++;
            std::cout << "Unable to add life (duplication), waiting for " << counter << "ms" << std::endl;
            usleep(counter * 1000);
        }
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


// For the UI
void Chunk::clearAddedLifes() {
    std::lock_guard<std::mutex> guard(entity_changes_mutex);

    this->lifesAdded.clear();
}

void Chunk::clearAddedEntities() {
    std::lock_guard<std::mutex> guard(entity_changes_mutex);

    this->entityAdded.clear();
}

void Chunk::clearToDeleteLifes() {
    std::lock_guard<std::mutex> guard(entity_changes_mutex);

    this->lifesToDelete.clear();
}

void Chunk::clearToDeleteEntities() {
    std::lock_guard<std::mutex> guard(entity_changes_mutex);

    this->entityToDelete.clear();
}











// Java hell

const std::string &Chunk::getStep() const {
    return step;
}

const Point &Chunk::getChunkPosition() const {
    return chunkPosition;
}

const std::vector<ActionDTO> &Chunk::getActions() const {
    return actions;
}

void Chunk::setActions(const std::vector<ActionDTO> &actions) {
    Chunk::actions = actions;
}


const std::vector<Life *> &Chunk::getLifes() const {
    return lifes;
}

void Chunk::setLifes(const std::vector<Life *> &lifes) {
    Chunk::lifes = lifes;
}

const std::vector<Entity *> &Chunk::getEntities() const {
    return entities;
}

void Chunk::setEntities(const std::vector<Entity *> &entities) {
    Chunk::entities = entities;
}

const std::vector<Life *> &Chunk::getLifesAdded() const {
    return lifesAdded;
}

void Chunk::setLifesAdded(const std::vector<Life *> &lifesAdded) {
    Chunk::lifesAdded = lifesAdded;
}

const std::vector<Life *> &Chunk::getLifesToDelete() const {
    return lifesToDelete;
}

void Chunk::setLifesToDelete(const std::vector<Life *> &lifesToDelete) {
    Chunk::lifesToDelete = lifesToDelete;
}

const std::vector<Entity *> &Chunk::getEntityAdded() const {
    return entityAdded;
}

void Chunk::setEntityAdded(const std::vector<Entity *> &entityAdded) {
    Chunk::entityAdded = entityAdded;
}

const std::vector<Entity *> &Chunk::getEntityToDelete() const {
    return entityToDelete;
}

void Chunk::setEntityToDelete(const std::vector<Entity *> &entityToDelete) {
    Chunk::entityToDelete = entityToDelete;
}

void Chunk::setStep(const std::string &step) {
    Chunk::step = step;
}

const std::vector<std::string> &Chunk::getSteps() const {
    return steps;
}

void Chunk::setSteps(const std::vector<std::string> &steps) {
    Chunk::steps = steps;
}
