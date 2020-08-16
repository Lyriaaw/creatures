//
// Created by Amalric Lombard de Buffières on 8/15/20.
//

#include <zconf.h>
#include "Chunk.h"
#include "../utils/perlin/PerlinNoise.h"

Chunk::Chunk(Point chunkPosition, CreatureNursery * nursery): chunkPosition(chunkPosition), nursery(nursery), step("init") {
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


void Chunk::generateEntityGrid() {
    std::chrono::system_clock::time_point start = std::chrono::system_clock::now();

    std::vector<std::string> steps;
    steps.clear();

    this->step = "ENTITY_GRID";
    steps.emplace_back("ENTITY_GRID");
    steps.emplace_back("ENERGY_GIVEAWAY");
    waitForNeighbours(steps);

    processImportedAndExportedLifes();

    int deltaX = chunkPosition.getX() * TILE_PER_CHUNK;
    int deltaY = chunkPosition.getY() * TILE_PER_CHUNK;

    for (int it = 0; it < TILE_PER_CHUNK; it++) {
        for (int jt = 0; jt < TILE_PER_CHUNK; jt++) {
            entityGrid.at(it).at(jt).clear();
        }
    }

    for (int it = 0; it < lifes.size(); it++) {
        Point position = lifes.at(it)->getEntity()->getPosition();
        Point tileCoordinate = position.getTileCoordinates();
        Point chunkCoordinates = position.getSimpleCoordinates();

        entityGrid.at(tileCoordinate.getX() - deltaX).at(tileCoordinate.getY() - deltaY).push_back(lifes.at(it)->getEntity());
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


    for (int it = 0; it < lifes.size(); it++) {
        Life *currentLife = lifes.at(it);
        Point entityPoint = currentLife->getEntity()->getPosition();
        Point tilePoint = entityPoint.getTileCoordinates();

        double releasedHeat = currentLife->giveawayEnergy();
        getTileAt(tilePoint.getX(), tilePoint.getY())->addHeat(releasedHeat);

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
    steps.emplace_back("CLIMATE_READY");
    steps.emplace_back("CLIMATE_START");
    waitForNeighbours(steps);

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

    this->step = "CLIMATE_READY";
    steps.clear();
    steps.emplace_back("CLIMATE_READY");
    steps.emplace_back("CLIMATE_SPREAD");
    waitForNeighbours(steps);

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



    this->step = "CLIMATE_SPREAD";
    steps.clear();
    steps.emplace_back("CLIMATE_SPREAD");
    steps.emplace_back("BRAIN_PROCESSING");
    waitForNeighbours(steps);


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
    std::vector<std::string> steps;
    steps.clear();

    this->step = "BRAIN_PROCESSING";
    steps.emplace_back("BRAIN_PROCESSING");
    steps.emplace_back("EXECUTE_ACTIONS");
    waitForNeighbours(steps);


    for (int it = 0; it < lifes.size(); it++) {
        Life *currentLife = lifes.at(it);
        currentLife->processSelectedTiles();
    }

    for (int it = 0; it < lifes.size(); it++) {
        Life *currentLife = lifes.at(it);


        std::vector<Entity *> accessibleEntities = getAccessibleEntities(currentLife->getSelectedTiles());
        std::vector<Tile *> accessibleTiles = getAccessibleTiles(currentLife->getSelectedTiles());

        currentLife->processSensors(accessibleEntities, accessibleTiles);


        currentLife->processBrain();

        std::vector<ActionDTO> currentCreatureActions = currentLife->executeExternalActions(accessibleEntities);
        actions.insert(actions.end(), currentCreatureActions.begin(), currentCreatureActions.end());

    }
}

void Chunk::executeCreaturesActions() {
    std::vector<std::string> steps;
    steps.clear();

    this->step = "EXECUTE_ACTIONS";
    steps.emplace_back("EXECUTE_ACTIONS");
    steps.emplace_back("MOVE_CREATURES");
    waitForNeighbours(steps);


    int captureGroundActions = 0;
    int captureHeatActions = 0;
    int duplicateActions = 0;
    int mateActions = 0;
    int eatActions = 0;

    int naturalMatingCount = 0;
    for (int it = 0; it < actions.size(); it++) {
        ActionDTO actionDto = actions.at(it);


        Life * performer = getLifeFromId(actionDto.getPerformerId(), true);
        Entity * subject = getEntityFromId(actionDto.getSubjectId(), true);

        if (!performer->getEnergyManagement()->isAlive()) {
            continue;
        }

        if (actionDto.getSubjectId() != 0 && (subject == nullptr || (subject != nullptr && !subject->isExists()))) {
            continue;
        }

        if (actionDto.getType() == "EAT") {
            performer->addEnergy(subject->getMass());
            subject->setMass(0.0);

            Life * foundLife = getLifeFromId(actionDto.getSubjectId(), true);
            if (foundLife != nullptr) {
                Point performerPosition = performer->getEntity()->getPosition();
                Point tilePosition = performerPosition.getTileCoordinates();

                getTileAt(tilePosition.getX(), tilePosition.getY())->addHeat(foundLife->getEnergyManagement()->getEnergy());
            }

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
}

void Chunk::moveCreatures() {

    std::vector<std::string> steps;
    steps.clear();

    this->step = "MOVE_CREATURES";
    steps.emplace_back("MOVE_CREATURES");
    waitForNeighbours(steps);


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

void Chunk::setNeighbour(int it, int jt, Chunk * neighbour) {
    neighbours.at(it + 1).at(jt + 1) = neighbour;
}


void Chunk::waitForNeighbours(std::vector<std::string> requestedSteps) {

    bool allNeighboursReady(true);

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
            usleep(100);
        }

    }while (!allNeighboursReady);
}



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

const std::string &Chunk::getStep() const {
    return step;
}

const Point &Chunk::getChunkPosition() const {
    return chunkPosition;
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

const std::vector<ActionDTO> &Chunk::getActions() const {
    return actions;
}

void Chunk::setActions(const std::vector<ActionDTO> &actions) {
    Chunk::actions = actions;
}








void Chunk::removeDeletedEntities() {
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



void Chunk::checkForLifeTransfer(Life * life) {
    int deltaX = chunkPosition.getX() * TILE_PER_CHUNK;
    int deltaY = chunkPosition.getY() * TILE_PER_CHUNK;

    Point lifeSpawnPoint = life->getEntity()->getPosition();
    Point tileLifeSpawnPoint = lifeSpawnPoint.getTileCoordinates();

    int tileX = tileLifeSpawnPoint.getX() - deltaX;
    int tileY = tileLifeSpawnPoint.getY() - deltaY;

    if (!(tileX < 0 || tileX >= TILE_PER_CHUNK || tileY < 0 || tileY >= TILE_PER_CHUNK)) {
        return;
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
}

void Chunk::transferLife(Life * life) {
    importedLifes.emplace_back(life);
}



void Chunk::handleCaptureHeat(Life * life, ActionDTO action) {

}

void Chunk::handleCaptureGround(Life * life, ActionDTO action) {
    int chunkReach = life->getEntity()->getSize() / 5.0;

    chunkReach = std::max(double(chunkReach), 1.0);


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

            double currentRatio = ratios[currentIndex] * 0.01;
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

    getTileAt(tilePoint.getX(), tilePoint.getY())->addHeat(wastedEnergy);
    getTileAt(tilePoint.getX(), tilePoint.getY())->processAddedHeat();

}


bool Chunk::handleDuplication(Life * life) {

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
        addLife(child);
        return true;
    }


    Point childCoordinate = child->getEntity()->getPosition();
    Point tileChildPosition = childCoordinate.getTileCoordinates();

    int deltaX = chunkPosition.getX() * TILE_PER_CHUNK;
    int deltaY = chunkPosition.getY() * TILE_PER_CHUNK;

    getRelativeTile(tileChildPosition.getX() - deltaX, tileChildPosition.getY() - deltaY, false)->addGround(totalGivenEnergy);


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

void Chunk::addLife(Life * life) {
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

        neighbours.at(ratioX + 1).at(ratioY + 1)->addLife(life);
        return;
    }



    lifes.emplace_back(life);
    lifesAdded.emplace_back(life);
    vegetals.emplace_back(life);
}


bool Chunk::handleMating(Life * father, int entityId) {
    Life * foundLife = getLifeFromId(entityId, true);
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

Life * Chunk::getLifeFromId(int id, bool askNeighbours) {
    for (int it = 0; it < this->lifes.size(); it++) {
        if (this->lifes.at(it)->getEntity()->getId() == id) {
            return this->lifes.at(it);
        }
    }

    if (!askNeighbours)
        return nullptr;

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

void Chunk::processImportedAndExportedLifes() {
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



const std::vector<Life *> &Chunk::getCreatures() const {
    return creatures;
}

void Chunk::setCreatures(const std::vector<Life *> &creatures) {
    Chunk::creatures = creatures;
}

const std::vector<Life *> &Chunk::getVegetals() const {
    return vegetals;
}

void Chunk::setVegetals(const std::vector<Life *> &vegetals) {
    Chunk::vegetals = vegetals;
}

void Chunk::clearAddedLifes() {
    this->lifesAdded.clear();
}

void Chunk::clearAddedEntities() {
    this->entityAdded.clear();
}
void Chunk::clearToDeleteLifes() {
    this->lifesToDelete.clear();
}

void Chunk::clearToDeleteEntities() {
    this->entityToDelete.clear();
}
