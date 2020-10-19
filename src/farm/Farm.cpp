//
// Created by Amalric Lombard de Buffières on 7/17/20.
//

#include "Farm.h"

#include <iostream>
#include <sstream>
#include <iomanip>
#include <unistd.h>


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
        initialLife->getEnergyCenter()->setAvailableEnergy(creatureEnergy);

        lifes.push_back(initialLife);
    }

    for (int it = 0; it < INITIAL_FOOD_COUNT; it++) {
        int x = distWidth(mt);
        int y = distHeight(mt);

        Point point(x, y);
        Point tilePosition = point.getTileCoordinates();


//        float foodSize = ((rand() % 300) / 100.f) + 2;
        float foodSize = 2;

        Entity * entity = new Entity(point);
        entity->setMass(2 * MASS_TO_SIZE_RATIO);

        Tile * currentTile = map->getTileAt(tilePosition.getX(), tilePosition.getY());
        entity->setColor(0.28f);
        entity->setBrightness(0.3f);

        entities.push_back(entity);
    }

    availableEnergy = 0.f;
    tickCount = 0;

    sortCreatures();
}


void Farm::Tick(bool paused) {



    if (!paused) {
        moveCreatures();
    }


    generateEntityGrid();

    multithreadBrainProcessing(paused);
    removeDeletedEntities();


    if (!paused){
        vegetalisation();
        populationControl();
    }




    sortCreatures();

    if (!paused) {
        aTickHavePassed();
        statistics();
    }




    tickEnd = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_time = tickEnd - tickStart;
    tickStart = std::chrono::system_clock::now();

    if (!paused) {
        double tickTime = elapsed_time.count();
        if (tickCount == 1) {
            dataAnalyser.getTickTime()->addValue(0);
        } else if (tickCount > 990 && tickCount < 1000) {
            dataAnalyser.getTickTime()->addValue(dataAnalyser.getTickTime()->getLastValue());
        } else {
            dataAnalyser.getTickTime()->addValue(tickTime);
        }


        dataAnalyser.getTickPerSecond()->addValue(1.0 / tickTime);
    }
}

void Farm::getLifeAt(int it) {

}

void Farm::multithreadBrainProcessing(bool paused) {
    std::chrono::system_clock::time_point start = std::chrono::system_clock::now();

    std::thread chunkThreads[lifes.size()];



    for (int it = 0; it < lifes.size(); it++) {

        Life * currentLife = lifes.at(it);

//        auto f = [](Life * life, Farm * farm, bool * paused) {

        currentLife->processSelectedChunks();


            std::vector<Entity *> accessibleEntities = this->getAccessibleEntities(currentLife->getSelectedChunks());
            std::vector<Tile *> accessibleTiles = this->getAccessibleTiles(currentLife->getSelectedChunks());
        currentLife->processSensors(accessibleEntities, accessibleTiles);


        currentLife->processBrain();



            if (!paused) {
                std::vector<ActionDTO> currentCreatureActions = currentLife->executeExternalActions(accessibleEntities);

                if (currentCreatureActions.size() > 0) {
                    this->executeCreaturesActions(currentCreatureActions);
                }
//                farm->addActions(currentCreatureActions);
            }


//        };

//        Life *currentLife = lifes.at(it);
//
//        int index = it;
//        chunkThreads[index] = std::thread(f, currentLife, this, &paused);
    }

//    int count = 0;
//    for (int it = 0; it < lifes.size(); it++) {
//        chunkThreads[it].join();
//        count++;
//    }

//    std::cout << "###############" << std::endl;
//    std::cout << "Finished: " << count << std::endl;
//    std::cout << "###############" << std::endl;


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

        checkAndHandleLifeDying(currentLife);
    }

    removeDeletedEntities();

    std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_time = end - start;
    dataAnalyser.getMoveCreaturesTime()->addValue(elapsed_time.count());
}

void Farm::addEntitiesToFarm(std::vector<Entity *> addedEntities) {
    std::lock_guard<std::mutex> guard(entities_mutex);
    entities.insert(entities.end(), addedEntities.begin(), addedEntities.end());
    entityAdded.insert(entityAdded.end(), addedEntities.begin(), addedEntities.end());
}

void Farm::addLifesToFarm(Life * newLife) {
    std::lock_guard<std::mutex> guard(lifes_mutex);
    lifes.emplace_back(newLife); // THIS LINE CRASHES EVERYTHING
    lifesAdded.emplace_back(newLife);
}



void Farm::executeCreaturesActions(std::vector<ActionDTO>  givenActions) {
//    std::chrono::system_clock::time_point start = std::chrono::system_clock::now();


    int naturalMatingCount = 0;
    for (int it = 0; it < givenActions.size(); it++) {
        ActionDTO actionDto = givenActions.at(it);

        Life * performer = getLifeFromId(actionDto.getPerformerId());

        if (performer == nullptr) {
            continue;
        }

        while (!std_mutex.try_lock()) {
            usleep(10000);
        }
        std_mutex.unlock();
        if (actionDto.getType() == "POOP") {
            return;
            handlePoop(performer);
            while (!std_mutex.try_lock()) {
                usleep(10000);
            }
            std_mutex.unlock();
            continue;
        }

        Entity * subject = getEntityFromId(actionDto.getSubjectId());

        if (subject->isExists() <= 0 || performer->isAlive() <= 0) {
            while (!std_mutex.try_lock()) {
                usleep(10000);
            }
            std_mutex.unlock();
            continue;
        }

        if (actionDto.getType() == "EAT") {
            handleEating(performer, subject);
        }

        if (actionDto.getType() == "MATE") {
            bool success = handleMating(performer, subject->getId());

            if (success)
                naturalMatingCount++;

        }

        if (actionDto.getType() == "BITE") {
            handleBiting(performer, subject);
        }
        while (!std_mutex.try_lock()) {
            usleep(10000);
        }
        std_mutex.unlock();
    }

//    actions.clear();
    dataAnalyser.getNaturalMatings()->addValue(naturalMatingCount);

//    std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
//    std::chrono::duration<double> elapsed_time = end - start;
//    dataAnalyser.getExecuteActionsTime()->addValue(elapsed_time.count());
}

void Farm::handleEating(Life * performer, Entity * subject) {

    subject->lockInteraction();
    performer->getEntity()->lockInteraction();

    if (!performer->isAlive() || !subject->isExists()) {
        subject->unlockInteraction();
        performer->getEntity()->unlockInteraction();
        return;
    }


    double wastedEnergy = performer->addEnergy(subject->getMass());
    subject->setMass(0.0);

    Point performerPoint = performer->getEntity()->getPosition();
    Point tilePoint = performerPoint.getTileCoordinates();

    map->getTileAt(tilePoint.getX(), tilePoint.getY())->addGround(wastedEnergy);

    Tile * tile = map->getTileAt(tilePoint.getX(), tilePoint.getY());

    Life * foundLife = getLifeFromId(subject->getId());
    if (foundLife != nullptr) {
        tile->addGround(foundLife->getEnergyCenter()->getWastedEnergy());
        tile->addHeat(foundLife->getEnergyCenter()->getAvailableEnergy());

        foundLife->getEnergyCenter()->setAvailableEnergy(0.0);
        foundLife->getEnergyCenter()->setWastedEnergy(0.0);

        checkAndHandleLifeDying(foundLife);


        subject->unlockInteraction();
        performer->getEntity()->unlockInteraction();

        std::lock_guard<std::mutex> guard(executed_actions_mutex);
        ActionDTO executedAction = ActionDTO(0, 0, "EAT_LIFE");
        executedAction.setTilePosition(tilePoint);
        executedAction.setTick(tickCount);
        executedActions.emplace_back(executedAction);
        return;
    }

    std::lock_guard<std::mutex> guard(executed_actions_mutex);
    ActionDTO executedAction = ActionDTO(0, 0, "EAT_ENTITY");
    executedAction.setTilePosition(tilePoint);
    executedAction.setTick(tickCount);
    executedActions.emplace_back(executedAction);

    subject->unlockInteraction();
    performer->getEntity()->unlockInteraction();

}

void Farm::checkAndHandleLifeDying(Life * life) {
    if (life->getEnergyCenter()->getAvailableEnergy() <= 0) {
        Point performerPoint = life->getEntity()->getPosition();
        Point tilePoint = performerPoint.getTileCoordinates();

        generateEntities(performerPoint, life->getEntity()->getColor(), 0.3f, 4000, life->getEntity()->getMass(), life->getEntity()->getSize());
        generateEntities(performerPoint, 0.04f, 0.2f, (0.1 * life->getEntity()->getSize() * MASS_TO_SIZE_RATIO), life->getEnergyCenter()->getWastedEnergy(), life->getEntity()->getSize());


        life->getEntity()->setMass(0.0);
        life->getEnergyCenter()->setWastedEnergy(0.0);
    }
}

void Farm::handleBiting(Life * performer, Entity * subject) {

    subject->lockInteraction();
    performer->getEntity()->lockInteraction();

    double mouthSize = performer->getEntity()->getSize() / 3.0;

    Point performerPoint = performer->getEntity()->getPosition();
    Point tilePoint = performerPoint.getTileCoordinates();

    Life * foundLife = getLifeFromId(subject->getId());
    if (foundLife != nullptr) {
        Tile * tile = map->getTileAt(tilePoint.getX(), tilePoint.getY());

        double takenEnergy = std::min(mouthSize * 10.0, foundLife->getEnergyCenter()->getAvailableEnergy());
        foundLife->getEnergyCenter()->removeAvailableEnergy(takenEnergy);
        tile->addHeat(takenEnergy);

        checkAndHandleLifeDying(foundLife);
        subject->unlockInteraction();
        performer->getEntity()->unlockInteraction();

        std::lock_guard<std::mutex> guard(executed_actions_mutex);
        ActionDTO executedAction = ActionDTO(0, 0, "BITE_LIFE");
        executedAction.setTilePosition(tilePoint);
        executedAction.setTick(tickCount);
        executedActions.emplace_back(executedAction);
        return;
    }



    random_device rd;
    mt19937 mt(rd());
    uniform_real_distribution<double> dist(-subject->getSize() * 2, subject->getSize() * 2);
    int x = dist(mt);
    int y = dist(mt);

    std::vector<Entity *> spawnedEntities;
    for (int it = 0; it < 2; it++) {
        Point position(subject->getPosition().getX() + x, subject->getPosition().getY() + y);

        if (position.getX() >= FARM_WIDTH) {
            position.setX(FARM_WIDTH - 1);
        }
        if (position.getY() >= FARM_HEIGHT) {
            position.setY(FARM_HEIGHT - 1);
        }
        if (position.getX() < 0) {
            position.setX(0);
        }
        if (position.getY() < 0) {
            position.setY(0);
        }


        Entity * bitenEntity = new Entity(position);
        bitenEntity->setColor(subject->getColor());
        bitenEntity->setBrightness(subject->getBrightness());
        bitenEntity->setMass(subject->getMass() / 2.0);

        spawnedEntities.emplace_back(bitenEntity);
    }
    addEntitiesToFarm(spawnedEntities);

    subject->setMass(0.0);


    subject->unlockInteraction();
    performer->getEntity()->unlockInteraction();

    std::lock_guard<std::mutex> guard(executed_actions_mutex);
    ActionDTO executedAction = ActionDTO(0, 0, "BITE_ENTITY");
    executedAction.setTilePosition(tilePoint);
    executedAction.setTick(tickCount);
    executedActions.emplace_back(executedAction);
}

void Farm::generateEntities(Point position, float color, float brightness, double maxSize, double totalEnergy, double spreadingRatio) {
    std::vector<Entity *> newEntities;
    do {
        double newEntityEnergy = std::min(totalEnergy, maxSize);

        float xRatio = ((rand() % 100) / 100.0f) * spreadingRatio;
        float yRatio = ((rand() % 100) / 100.0f) * spreadingRatio;

        Point entityPosition = Point(position.getX() + xRatio, position.getY() + yRatio);
        if (entityPosition.getX() >= FARM_WIDTH) {
            entityPosition.setX(FARM_WIDTH - 1);
        }
        if (entityPosition.getY() >= FARM_HEIGHT) {
            entityPosition.setY(FARM_HEIGHT - 1);
        }
        if (entityPosition.getX() < 0) {
            entityPosition.setX(0);
        }
        if (entityPosition.getY() < 0) {
            entityPosition.setY(0);
        }



        totalEnergy -= newEntityEnergy;

        Entity * poop = new Entity(entityPosition);
        poop->setColor(color);
        poop->setBrightness(brightness);
        poop->setMass(newEntityEnergy);
        newEntities.emplace_back(poop);

    } while (totalEnergy > 0.0);
    addEntitiesToFarm(newEntities);
}

void Farm::handlePoop(Life * subject) {
    if (subject->getEnergyCenter()->getWastedEnergy() == 0) {
        return;
    }

    Point position = subject->getEntity()->getPosition();

    double maxPoopSize = 5;

    double poopedEnergy = subject->getEnergyCenter()->getWastedEnergy();
    generateEntities(position, 0.04f, 0.2f, (0.1 * subject->getEntity()->getSize() * MASS_TO_SIZE_RATIO), poopedEnergy, subject->getEntity()->getSize());


    subject->getEnergyCenter()->setWastedEnergy(0.0);

    std::lock_guard<std::mutex> guard(executed_actions_mutex);
    ActionDTO executedAction = ActionDTO(0, 0, "POOP");
    executedAction.setTilePosition(position.getTileCoordinates());
    executedActions.emplace_back(executedAction);
}


bool Farm::handleMating(Life * father, int entityId) {
    Life * foundLife = getLifeFromId(entityId);
    if (foundLife == nullptr) {
        return false;
    }


    father->getEntity()->lockInteraction();
    foundLife->getEntity()->lockInteraction();

    if (!father->isAlive() || !foundLife->isAlive()) {
        father->getEntity()->unlockInteraction();
        foundLife->getEntity()->unlockInteraction();
        return false;
    }


    bool fatherCanReproduce = father->getEntity()->getMass() > father->getEnergyCenter()->getMaxMass() / 3.f && father->getEntity()->getAge() > 10;
    bool motherCanReproduce = foundLife->getEntity()->getMass() > foundLife->getEnergyCenter()->getMaxMass() / 3.f && foundLife->getEntity()->getAge() > 10;

    if (!fatherCanReproduce || !motherCanReproduce) {
        father->getEntity()->unlockInteraction();
        foundLife->getEntity()->unlockInteraction();
        return false;
    }
//
    Life * child = this->nursery->Mate(father, foundLife);

    double givenEnergyToChildGoal = child->getEnergyCenter()->getMaxMass() / 2.f;

    double givenFatherEnergy = std::min(father->getEntity()->getMass() / 4.0, givenEnergyToChildGoal / 2.0);
    double givenMotherEnergy = std::min(foundLife->getEntity()->getMass() / 4.0, givenEnergyToChildGoal / 2.0);

    double actualGivenFatherEnergy = father->getEntity()->removeMass(givenFatherEnergy);
    double actualGivenMotherEnergy = foundLife->getEntity()->removeMass(givenMotherEnergy);

    if (givenFatherEnergy != actualGivenFatherEnergy || givenMotherEnergy != actualGivenMotherEnergy) {
        std::cout << "Wrong energy given" << std::endl;
    }


    double totalGivenEnergy = actualGivenFatherEnergy + actualGivenMotherEnergy;

    checkAndHandleLifeDying(father);
    checkAndHandleLifeDying(foundLife);


    Point childCoordinate = child->getEntity()->getPosition();
    Point tileChildPosition = childCoordinate.getTileCoordinates();

    if (totalGivenEnergy > givenEnergyToChildGoal / 2.0) {
        child->getEntity()->setMass(totalGivenEnergy / 2.0);
        child->getEnergyCenter()->setAvailableEnergy(totalGivenEnergy / 2.0);
        addLifesToFarm(child);

        father->getEntity()->unlockInteraction();
        foundLife->getEntity()->unlockInteraction();


        std::lock_guard<std::mutex> guard(executed_actions_mutex);
        ActionDTO executedAction = ActionDTO(0, 0, "MATE");
        executedAction.setTilePosition(tileChildPosition);
        executedActions.emplace_back(executedAction);

        return true;
    }



    father->getEntity()->unlockInteraction();
    foundLife->getEntity()->unlockInteraction();


    map->getTileAt(tileChildPosition.getX(), tileChildPosition.getY())->addGround(totalGivenEnergy);

//    if (givenMotherEnergy + givenFatherEnergy == 0) {
//        std::cout << "New child " << child->getCreature()->getId() << " Energy: " << givenMotherEnergy + givenFatherEnergy << std::endl;
//    } else {
//        std::cout << "New Child " << std::endl;
//
//    }





    return false;
}



void Farm::populationControl() {
    std::lock_guard<std::mutex> guard(lifes_mutex);

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


        addLifesToFarm(child);
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

        if (entity->getMass() < 100) {
            tile->addGround(entity->getMass());
            entity->setMass(0.0);
        } else {
            double transferedMass = 2.0 * VEGETALISATION_RATE;
            tile->addGround(transferedMass);
            entity->removeMass(transferedMass);
        }

    }

    removeDeletedEntities();
}

void Farm::vegetalisation() {
    std::lock_guard<std::mutex> guard(entities_mutex);
    std::chrono::system_clock::time_point start = std::chrono::system_clock::now();

    if (VEGETALISATION_RATE != 1 && tickCount % VEGETALISATION_RATE != 0) {
        removeDeletedEntities();
        std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_time = end - start;
        dataAnalyser.getVegetalisationTime()->addValue(elapsed_time.count());
        return;
    }

    map->processClimate();
    handleDecay();

    random_device rd;
    mt19937 mt(rd());
    uniform_real_distribution<double> distWidth(-10, TILE_SIZE + 10);
    uniform_real_distribution<double> distHeight(-10, TILE_SIZE + 10);


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
    std::chrono::system_clock::time_point start = std::chrono::system_clock::now();

    for (int it = 0; it < lifes.size(); it++) {
        lifes.at(it)->getEntity()->aTickHavePassed();
    }
    for (int it = 0; it < entities.size(); it++) {
        entities.at(it)->aTickHavePassed();
    }
    tickCount++;

    std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_time = end - start;
    dataAnalyser.getATickHavePassedTime()->addValue(elapsed_time.count());
}


void Farm::analyseColors() {
    std::vector<double> sortResult;

    std::vector<Life *> tmpLifes = this->lifes;

    while (!tmpLifes.empty()) {

        int biggestIndex = -1;
        float biggestScore = 0.f;

        for (int it = 0; it < tmpLifes.size(); it++) {
            if (tmpLifes.at(it)->getEntity()->getColor() >= biggestScore) {
                biggestIndex = it;
                biggestScore = tmpLifes.at(it)->getEntity()->getColor();
            }
        }

        if (biggestIndex == -1) {
            std::cout << "Error while sorting creatures by score - Life will probably crash" << std::endl;
        }

        sortResult.emplace_back(tmpLifes.at(biggestIndex)->getEntity()->getColor());
        tmpLifes.erase(tmpLifes.begin() + biggestIndex);
    }

    dataAnalyser.getColors()->addTick(sortResult);
}

void Farm::statistics() {
    std::chrono::system_clock::time_point start = std::chrono::system_clock::now();

    analyseColors();

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


    double totalInputNeurons = 0;
    double totalOutputNeurons = 0;
    double totalLinks = 0;


    double totalCreaturesEnergy = 0.f;
    double totalCreaturesMass = 0.f;
    double totalCreaturesWasted = 0.f;
    double totalFoodsEnergy = 0.f;

    for (int it = 0; it < lifes.size(); it++) {
        Life * currentLife = lifes.at(it);
        totalCreaturesEnergy += currentLife->getEnergyCenter()->getAvailableEnergy();
        totalCreaturesMass += currentLife->getEntity()->getMass();
        totalCreaturesWasted += currentLife->getEnergyCenter()->getWastedEnergy();

        totalInputNeurons += currentLife->getBrain()->getNeurons().size();
        totalOutputNeurons += currentLife->getBrain()->getOutputNeurons().size();
        totalLinks += currentLife->getBrain()->getLinks().size();
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


    dataAnalyser.getAverageInputNeurons()->addValue(totalInputNeurons / double(lifes.size()));
    dataAnalyser.getAverageOutputNeurons()->addValue(totalOutputNeurons / double(lifes.size()));
    dataAnalyser.getAverageLinks()->addValue(totalLinks / double(lifes.size()));

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
//    totalTime += dataAnalyser.getExecuteActionsTime()->getLastValue();
    totalTime += dataAnalyser.getMoveCreaturesTime()->getLastValue();
    totalTime += dataAnalyser.getPopulationControlTime()->getLastValue();
    totalTime += dataAnalyser.getVegetalisationTime()->getLastValue();
    totalTime += dataAnalyser.getCreatureSortingTime()->getLastValue();
    totalTime += dataAnalyser.getATickHavePassedTime()->getLastValue();

    std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_time = end - start;
    double statisticsTime = elapsed_time.count();

    dataAnalyser.getStatisticsTime()->addValue(statisticsTime);
    totalTime += statisticsTime;

    dataAnalyser.getTotalTime()->addValue(totalTime);

    dataAnalyser.getZero()->addValue(0);
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
    std::lock_guard<std::mutex> guard2(lifes_mutex);


    std::vector<Life *> newLifes;
    for (int it = 0; it < lifes.size(); it++) {
        lifes.at(it)->getEntity()->lockInteraction();

        if (lifes.at(it)->isAlive()) {
            newLifes.emplace_back(lifes.at(it));
        } else {
            lifesToDelete.emplace_back(lifes.at(it));
        }

        lifes.at(it)->getEntity()->unlockInteraction();
    }

    lifes.clear();
    lifes = newLifes;

    std::vector<Entity *> newEntities;
    for (int it = 0; it < entities.size(); it++) {
        Entity * currentEntity = entities.at(it);
        currentEntity->lockInteraction();
        if (currentEntity->isExists()) {
            newEntities.emplace_back(currentEntity);
        } else {
            entityToDelete.emplace_back(currentEntity);
        }

        currentEntity->unlockInteraction();
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
    std::chrono::system_clock::time_point start = std::chrono::system_clock::now();

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

    std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_time = end - start;
    double statisticsTime = elapsed_time.count();

    dataAnalyser.getCreatureSortingTime()->addValue(statisticsTime);
}



Life * Farm::getLifeFromId(int id) {
    std::lock_guard<std::mutex> guard3(lifes_mutex);

    for (int it = 0; it < this->lifes.size(); it++) {
        if (this->lifes.at(it)->getEntity()->getId() == id) {
            return this->lifes.at(it);
        }
    }

    return nullptr;
}

Entity * Farm::getEntityFromId(int id) {
    std::lock_guard<std::mutex> guard3(entities_mutex);
    std::lock_guard<std::mutex> guard2(lifes_mutex);

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

vector<Life *> &Farm::getLifes() {
    std::lock_guard<std::mutex> guard3(lifes_mutex);

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

vector<Entity *> Farm::getAndClearEntitiesToDelete() {
//    std::lock_guard<std::mutex> guard(entities_mutex);

    vector<Entity *> currentEntitiesToDelete;
//    currentEntitiesToDelete.insert(currentEntitiesToDelete.begin(), entityToDelete.begin(), entityToDelete.end());
//    entityToDelete.clear();

    return currentEntitiesToDelete;
}

vector<Life *> Farm::getAndClearLifesToDelete() {
//    std::lock_guard<std::mutex> guard(lifes_mutex);

    vector<Life *> currentLifesToDelete;
//    currentLifesToDelete.insert(currentLifesToDelete.begin(), lifesToDelete.begin(), lifesToDelete.end());
//    lifesToDelete.clear();

    return currentLifesToDelete;
}

vector<Entity *> Farm::getAndClearEntitiesToAdd() {
    std::lock_guard<std::mutex> guard(entities_mutex);

    vector<Entity *> currentEntitiesToAdd;
    currentEntitiesToAdd.insert(currentEntitiesToAdd.begin(), entityAdded.begin(), entityAdded.end());
    entityAdded.clear();

    return currentEntitiesToAdd;
}

vector<Life *> Farm::getAndClearLifesToAdd() {
    std::lock_guard<std::mutex> guard(lifes_mutex);

    vector<Life *> currentLifesToAdd;
    currentLifesToAdd.insert(currentLifesToAdd.begin(), lifesAdded.begin(), lifesAdded.end());
    lifesAdded.clear();

    return currentLifesToAdd;
}

vector<ActionDTO> Farm::getAndClearExecutedActions() {
    std::lock_guard<std::mutex> guard(executed_actions_mutex);

    vector<ActionDTO> currentExecutedActions;
    currentExecutedActions.insert(currentExecutedActions.begin(), executedActions.begin(), executedActions.end());
    executedActions.clear();

    return currentExecutedActions;
}




int Farm::getTickCount() const {
    return tickCount;
}



