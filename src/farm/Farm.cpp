//
// Created by Amalric Lombard de Buffières on 7/17/20.
//

#include "Farm.h"

#include <iostream>
#include <sstream>
#include <iomanip>


using namespace std;

Farm::Farm(){
    farmControl = new FarmControl();
    nursery = new CreatureNursery();


    tickStart = std::chrono::system_clock::now();
    tickEnd = std::chrono::system_clock::now();
}

void Farm::initLifesRunners() {
    for (int it = 0; it < CONCURRENT_LIFE_RUNNER; it++) {
        LifesRunner * lifesRunner = new LifesRunner();
        lifesRunner->setGenerateEntities([&](Point position, float color, float brightness, double maxSize, double totalEnergy, double spreadingRatio) {
            this->generateEntities(position, color, brightness, maxSize, totalEnergy, spreadingRatio);
        });

        lifesRunner->setGetAccessibleEntities([&](std::vector<Point> selectedChunks) {
           return getAccessibleEntities(selectedChunks);
        });

        lifesRunner->setGetAccessibleTiles([&](Life * life, std::vector<Point> selectedChunks) {
           return getAccessibleTiles(life, selectedChunks);
        });

        lifesRunner->setRecordExecutedAction([&](ActionDTO action) {
           this->recordExecutedAction(action);
        });

        lifesRunner->setGetLifeFromId([&](int id) {
           return this->getLifeFromId(id);
        });

        lifesRunner->setAddLifeToFarm([&](Life * life) {
            this->addLifeToFarm(life);
        });

        lifesRunner->setMap(map);
        lifesRunner->setCreatureNursery(nursery);

        lifesRunners.emplace_back(lifesRunner);
    }
}

void Farm::InitFromRandom() {
    random_device rd;
    mt19937 mt(rd());
    uniform_real_distribution<double> distWidth(0, FARM_WIDTH);
    uniform_real_distribution<double> distHeight(0, FARM_HEIGHT);

    map = new Map(farmControl);
    map->setRecordAddedEntitiesToFarm([&](std::vector<Entity *> entities) {
        this->addedEntitiesToFarm(entities);
    });
    map->initRandomMap();


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

    initLifesRunners();
    std::uniform_real_distribution<float> distMovement(-1, 1);
    for (int it = 0; it < INITIAL_CREATURE_COUNT; it++) {
        Life * initialLife = nursery->generateCreatureFromRandom();

        float creatureEnergy = initialLife->getEnergyCenter()->getMaxMass() / 2.0;
        initialLife->getEntity()->setMass(creatureEnergy);
        initialLife->getEnergyCenter()->setAvailableEnergy(creatureEnergy);

        lifesRunners.at(it % CONCURRENT_LIFE_RUNNER)->addLife(initialLife);
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

        addEntityToFarm(entity);
    }

    availableEnergy = 0.f;
    tickCount = 0;

    sortCreatures();
}

void Farm::addLifeToFarm(Life * life) {
    getLessLoadedRunner()->addLife(life);

    std::lock_guard<std::mutex> guard(add_mutex);
    lifesAdded.emplace_back(life);

}


void Farm::Tick(bool paused) {


    if (!paused) {
        for (auto const& runner : lifesRunners) {
            runner->setTick(tickCount);
        }


        moveCreatures();
    }



    generateEntityGrid();

    multithreadBrainProcessing(&paused);


    if (!paused) {
        executeCreaturesActions();
        populationControl();
    }




    sortCreatures();

    if (!paused) {
        aTickHavePassed();
        statistics();
    }



    if (map->getTick() != 0) {
        double mapTickDifference = map->getTick() - tickCount;
        map->setSpeedCorrectionRatio(mapTickDifference);
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

void Farm::multithreadBrainProcessing(bool *paused) {
    std::chrono::system_clock::time_point start = std::chrono::system_clock::now();


    double chunks(0.0);
    double sensors(0.0);
    double brains(0.0);
    double actions(0.0);
    double accessibleEntitiesTime(0.0);
    double poopCount(0.0);
    double eatCount(0.0);
    double biteCount(0.0);
    double pheromoneCount(0.0);
    double biteLifeCount(0.0);
    double eatLifeCount(0.0);
    int mateFailureCount = 0;
    int mateSuccessCount = 0;
    int naturalMatingCount = 0;

    std::thread chunkThreads[lifesRunners.size()];
    for (int it = 0; it < lifesRunners.size(); it++) {
        auto f = [](LifesRunner * currentLifeRunner, Farm * farm, bool * paused) {
            currentLifeRunner->brainProcessing(*paused);
        };

        int index = it;
        chunkThreads[index] = std::thread(f, lifesRunners.at(it), this, paused);
    }



    for (int it = 0; it < lifesRunners.size(); it++) {
        chunkThreads[it].join();
        chunks += lifesRunners.at(it)->getDataAnalyser().getChunkSelection()->getLastValue();
        sensors += lifesRunners.at(it)->getDataAnalyser().getSensorProcessing()->getLastValue();
        brains += lifesRunners.at(it)->getDataAnalyser().getBrainProcessing()->getLastValue();
        actions += lifesRunners.at(it)->getDataAnalyser().getExternalActions()->getLastValue();
        poopCount += lifesRunners.at(it)->getDataAnalyser().getPoopCount()->getLastValue();
        accessibleEntitiesTime += lifesRunners.at(it)->getDataAnalyser().getAccessibleEntities()->getLastValue();
        eatCount += lifesRunners.at(it)->getDataAnalyser().getEatCount()->getLastValue();
        biteCount += lifesRunners.at(it)->getDataAnalyser().getBiteCount()->getLastValue();
        pheromoneCount += lifesRunners.at(it)->getDataAnalyser().getPheromoneCount()->getLastValue();
        biteLifeCount += lifesRunners.at(it)->getDataAnalyser().getBiteLifeCount()->getLastValue();
        eatLifeCount += lifesRunners.at(it)->getDataAnalyser().getEatLifeCount()->getLastValue();
        mateFailureCount += lifesRunners.at(it)->getDataAnalyser().getMateFailureCount()->getLastValue();
        mateSuccessCount += lifesRunners.at(it)->getDataAnalyser().getMateSuccessCount()->getLastValue();
        naturalMatingCount += lifesRunners.at(it)->getDataAnalyser().getNaturalMatings()->getLastValue();
    }




    std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_time = end - start;
    dataAnalyser.getBrainProcessingTime()->addValue(elapsed_time.count());

    dataAnalyser.getChunkSelection()->addValue(chunks);
    dataAnalyser.getSensorProcessing()->addValue(sensors);
    dataAnalyser.getBrainProcessing()->addValue(brains);
    dataAnalyser.getExternalActions()->addValue(actions);
    dataAnalyser.getAccessibleEntities()->addValue(accessibleEntitiesTime);
    dataAnalyser.getPoopCount()->addValue(poopCount);
    dataAnalyser.getEatCount()->addValue(eatCount);
    dataAnalyser.getBiteCount()->addValue(biteCount);
    dataAnalyser.getPheromoneCount()->addValue(pheromoneCount);
    dataAnalyser.getBiteLifeCount()->addValue(biteLifeCount);
    dataAnalyser.getEatLifeCount()->addValue(eatLifeCount);
    dataAnalyser.getMateFailureCount()->addValue(mateFailureCount);
    dataAnalyser.getMateSuccessCount()->addValue(mateSuccessCount);
    dataAnalyser.getNaturalMatings()->addValue(naturalMatingCount);
}



void Farm::brainProcessing(bool paused) {
//    std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
//
//
//    std::chrono::system_clock::time_point chunkProcessingStart = std::chrono::system_clock::now();
//    for (int it = 0; it < lifes.size(); it++) {
//        Life *currentLife = lifes.at(it);
//        currentLife->processSelectedChunks();
//    }
//    std::chrono::system_clock::time_point chunkProcessingEnd = std::chrono::system_clock::now();
//    std::chrono::duration<double> elapsed_timeChunk = chunkProcessingEnd - chunkProcessingStart;
//    dataAnalyser.getChunkSelection()->addValue(elapsed_timeChunk.count());
//
//
//
//    std::chrono::system_clock::time_point sensorProcessingStart = std::chrono::system_clock::now();
//    for (int it = 0; it < lifes.size(); it++) {
//        Life *currentLife = lifes.at(it);
//
//        std::vector<Entity *> accessibleEntities = getAccessibleEntities(currentLife->getSelectedChunks());
//        std::vector<Tile *> accessibleTiles = getAccessibleTiles(currentLife, currentLife->getSelectedChunks());
//        currentLife->processSensors(accessibleEntities, accessibleTiles);
//    }
//    std::chrono::system_clock::time_point sensorProcessingEnd = std::chrono::system_clock::now();
//    std::chrono::duration<double> elapsed_timeSensor = sensorProcessingEnd - sensorProcessingStart;
//    dataAnalyser.getSensorProcessing()->addValue(elapsed_timeSensor.count());
//
//
//
//
//    std::chrono::system_clock::time_point brainProcessingStart = std::chrono::system_clock::now();
//
//    for (int it = 0; it < lifes.size(); it++) {
//        Life *currentLife = lifes.at(it);
//
//        currentLife->processBrain();
//    }
//    std::chrono::system_clock::time_point brainProcessingEnd = std::chrono::system_clock::now();
//    std::chrono::duration<double> elapsed_timeBrain = brainProcessingEnd - brainProcessingStart;
//    dataAnalyser.getBrainProcessing()->addValue(elapsed_timeBrain.count());
//
//
//    if (paused) {
//        std::chrono::system_clock::time_point externalActionsStart = std::chrono::system_clock::now();
//        std::chrono::system_clock::time_point externalActionsEnd = std::chrono::system_clock::now();
//        std::chrono::duration<double> elapsed_timeActions = externalActionsEnd - externalActionsStart;
//        dataAnalyser.getExternalActions()->addValue(elapsed_timeActions.count());
//
//        std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
//        std::chrono::duration<double> elapsed_time = end - start;
//        dataAnalyser.getBrainProcessingTime()->addValue(elapsed_time.count());
//        return;
//    }
//
//    std::chrono::system_clock::time_point externalActionsStart = std::chrono::system_clock::now();
//    for (int it = 0; it < lifes.size(); it++) {
//        Life *currentLife = lifes.at(it);
//
//        std::vector<Entity *> accessibleEntities = getAccessibleEntities(currentLife->getSelectedChunks());
//
//        std::vector<ActionDTO> currentCreatureActions = currentLife->executeExternalActions(accessibleEntities);
//        actions.insert(actions.end(), currentCreatureActions.begin(), currentCreatureActions.end());
//    }
//    std::chrono::system_clock::time_point externalActionsEnd = std::chrono::system_clock::now();
//    std::chrono::duration<double> elapsed_timeActions = externalActionsEnd - externalActionsStart;
//    dataAnalyser.getExternalActions()->addValue(elapsed_timeActions.count());
//
//
//
//    std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
//    std::chrono::duration<double> elapsed_time = end - start;
//    dataAnalyser.getBrainProcessingTime()->addValue(elapsed_time.count());
}


void Farm::moveCreatures() {
    std::chrono::system_clock::time_point start = std::chrono::system_clock::now();


    std::thread chunkThreads[lifesRunners.size()];
    for (int it = 0; it < lifesRunners.size(); it++) {
        auto f = [](LifesRunner * currentLifeRunner, Farm * farm) {
            currentLifeRunner->moveCreatures();
        };

        int index = it;
        chunkThreads[index] = std::thread(f, lifesRunners.at(it), this);
    }



    for (int it = 0; it < lifesRunners.size(); it++) {
        chunkThreads[it].join();
    }

    std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_time = end - start;
    dataAnalyser.getMoveCreaturesTime()->addValue(elapsed_time.count());
}



void Farm::executeCreaturesActions() {

    std::chrono::system_clock::time_point start = std::chrono::system_clock::now();

    std::vector<ActionDTO> actions;
    for (int it = 0; it < lifesRunners.size(); it++) {
        std::vector<ActionDTO> runnerActions = lifesRunners.at(it)->getActions();
        actions.insert(actions.end(), runnerActions.begin(), runnerActions.end());
        lifesRunners.at(it)->clearActions();
    }

    int poopCount = 0;
    int pheromoneCount = 0;
    int eatCount = 0;
    int eatLifeCount = 0;
    int mateFailureCount = 0;
    int mateSuccessCount = 0;
    int biteCount = 0;
    int biteLifeCount = 0;



    removeDeadLifes();

    actions.clear();

    std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_time = end - start;
    dataAnalyser.getExecuteActionsTime()->addValue(elapsed_time.count());
}

void Farm::handlePheromoneEmission(Life * performer, ActionDTO action) {
    Point performerPoint = performer->getEntity()->getPosition();
    Point tilePoint = performerPoint.getTileCoordinates();

    map->getTileAt(tilePoint.getX(), tilePoint.getY())->addPheromone(action.getPheromoneEmissionColor(), performer->getEntity()->getSize());
}

void Farm::handleEating(Life * performer, Entity * subject) {
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


}

void Farm::handleBitingLife(Life * performer, ActionDTO action) {

    double mouthSize = performer->getEntity()->getSize() / 3.0;

    Point performerPoint = performer->getEntity()->getPosition();
    Point tilePoint = performerPoint.getTileCoordinates();

    Life * foundLife = getLifeFromId(action.getSubjectId());
    if (foundLife != nullptr) {
        Tile * tile = map->getTileAt(tilePoint.getX(), tilePoint.getY());

        double takenEnergy = std::min(mouthSize * 10.0, foundLife->getEnergyCenter()->getAvailableEnergy());
        foundLife->getEnergyCenter()->removeAvailableEnergy(takenEnergy);
        tile->addTmpHeat(takenEnergy);

        checkAndHandleLifeDying(foundLife);

        ActionDTO executedAction = ActionDTO(0, 0, "BITE_LIFE");
        executedAction.setTilePosition(tilePoint);
        executedAction.setTick(tickCount);
        std::lock_guard<std::mutex> guard(executed_actions_mutex);
        executedActions.emplace_back(executedAction);
        return;
    }


}

void Farm::handleBiting(Life * performer, Entity * subject) {

    double mouthSize = performer->getEntity()->getSize() / 3.0;

    Point performerPoint = performer->getEntity()->getPosition();
    Point tilePoint = performerPoint.getTileCoordinates();

    random_device rd;
    mt19937 mt(rd());
    uniform_real_distribution<double> dist(-subject->getSize() * 2, subject->getSize() * 2);
    int x = dist(mt);
    int y = dist(mt);

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

        addEntityToFarm(bitenEntity);
        entityAdded.emplace_back(bitenEntity);
    }

    subject->setMass(0.0);


    std::lock_guard<std::mutex> guard(executed_actions_mutex);
    ActionDTO executedAction = ActionDTO(0, 0, "BITE_ENTITY");
    executedAction.setTilePosition(tilePoint);
    executedAction.setTick(tickCount);
    executedActions.emplace_back(executedAction);
}

void Farm::generateEntities(Point position, float color, float brightness, double maxSize, double totalEnergy, double spreadingRatio) {
    std::vector<Entity *> generatedEntities;

    do {
        double newEntityEnergy = std::min(totalEnergy, maxSize);

        float xRatio = (((rand() % 200) / 100.0f) * spreadingRatio) - spreadingRatio;
        float yRatio = (((rand() % 200) / 100.0f) * spreadingRatio) - spreadingRatio;

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

        addEntityToFarm(poop);
        generatedEntities.emplace_back(poop);

    } while (totalEnergy > 0.0);


    if (generatedEntities.empty()) {
        return;
    }

    std::lock_guard<std::mutex> guard(add_mutex);
    entityAdded.insert(entityAdded.end(), generatedEntities.begin(), generatedEntities.end());
}

void Farm::addEntityToFarm(Entity * entity) {
    Point entityCoordinates = entity->getTileCoordinates();
    map->getTileAt(entityCoordinates.getX(), entityCoordinates.getY())->addEntity(entity);
}

void Farm::addedEntitiesToFarm(std::vector<Entity *> entities) {
    std::lock_guard<std::mutex> guard(add_mutex);
    entityAdded.insert(entityAdded.end(), entities.begin(), entities.end());
}

void Farm::handlePoop(Life * subject) {
    if (subject->getEnergyCenter()->getWastedEnergy() == 0) {
        return;
    }
    std::cout << "Old poop" << std::endl;

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

void Farm::recordExecutedAction(ActionDTO action) {
    std::lock_guard<std::mutex> guard(executed_actions_mutex);
    executedActions.emplace_back(action);
}


bool Farm::handleMating(Life * father, int entityId) {
    Life * foundLife = getLifeFromId(entityId);
    if (foundLife == nullptr) {
        return false;
    }


    bool fatherCanReproduce = father->getEntity()->getMass() > father->getEnergyCenter()->getMaxMass() / 3.f && father->getEntity()->getAge() > 10;
    bool motherCanReproduce = foundLife->getEntity()->getMass() > foundLife->getEnergyCenter()->getMaxMass() / 3.f && foundLife->getEntity()->getAge() > 10;

    if (!fatherCanReproduce || !motherCanReproduce) {
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
        getLessLoadedRunner()->addLife(child);
        lifesAdded.emplace_back(child);


        std::lock_guard<std::mutex> guard(executed_actions_mutex);
        ActionDTO executedAction = ActionDTO(0, 0, "MATE");
        executedAction.setTilePosition(tileChildPosition);
        executedActions.emplace_back(executedAction);

        return true;
    }



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
    std::lock_guard<std::mutex> guard(add_mutex);

    std::chrono::system_clock::time_point start = std::chrono::system_clock::now();

    std::vector<Life *> lifes = getLifes();

    if (lifes.size() > int(INITIAL_CREATURE_COUNT / 2) - (INITIAL_CREATURE_COUNT * 0.05)) {
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

    int newConnectorNeeded = int(INITIAL_CREATURE_COUNT / 2) + (INITIAL_CREATURE_COUNT * 0.05) - lifes.size();

    float totalEnergyRemoved = 0.f;
    for (int it = 0; it < newConnectorNeeded; it++) {
        int fatherIndex = rand() % selectedParentCount;
        int motherIndex = rand() % selectedParentCount;

        Life * father = sortedConnectors.at(fatherIndex);
        Life * mother = sortedConnectors.at(motherIndex);

        Life * child = this->nursery->Mate(father, mother);
        child->getEntity()->setMass(child->getEnergyCenter()->getMaxMass() / 3.01f);
        child->getEnergyCenter()->setAvailableEnergy(child->getEnergyCenter()->getMaxMass() / 3.01f);
        Entity * childCreature = child->getEntity();

//        float childSpawnX = distWidth(mt);
//        float childSpawnY = distHeight(mt);
//        Point childCreaturePosition = Point(childSpawnX, childSpawnY);
//
//        childCreature->setPosition(childCreaturePosition);

        totalEnergyRemoved += child->getEntity()->getMass() + child->getEnergyCenter()->getAvailableEnergy();

        getLessLoadedRunner()->addLife(child);

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


    std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_time = end - start;
    dataAnalyser.getVegetalisationTime()->addValue(elapsed_time.count());
}

void Farm::aTickHavePassed() {
    std::chrono::system_clock::time_point start = std::chrono::system_clock::now();

    std::vector<Life *> lifes = getLifes();
    for (int it = 0; it < lifes.size(); it++) {
        lifes.at(it)->getEntity()->aTickHavePassed();
    }


    tickCount++;

    std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_time = end - start;
    dataAnalyser.getATickHavePassedTime()->addValue(elapsed_time.count());
}


void Farm::analyseColors() {
    std::vector<double> sortResult;

    std::vector<Life *> tmpLifes = getLifes();

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



void Farm::handleNoStatistics() {
    std::chrono::system_clock::time_point start = std::chrono::system_clock::now();

    dataAnalyser.getPopulation()->addValue(dataAnalyser.getPopulation()->getLastValue());
    dataAnalyser.getAverageScore()->addValue(dataAnalyser.getAverageScore()->getLastValue());
    dataAnalyser.getBestScore()->addValue(dataAnalyser.getBestScore()->getLastValue());
    dataAnalyser.getFirstQuartileScore()->addValue(dataAnalyser.getFirstQuartileScore()->getLastValue());
    dataAnalyser.getMedianScore()->addValue(dataAnalyser.getMedianScore()->getLastValue());
    dataAnalyser.getLastQuartileScore()->addValue(dataAnalyser.getLastQuartileScore()->getLastValue());
    dataAnalyser.getAvailableEnergy()->addValue(dataAnalyser.getAvailableEnergy()->getLastValue());
    dataAnalyser.getFoodEnergy()->addValue(dataAnalyser.getFoodEnergy()->getLastValue());
    dataAnalyser.getCreaturesEnergy()->addValue(dataAnalyser.getCreaturesEnergy()->getLastValue());
    dataAnalyser.getCreaturesMass()->addValue(dataAnalyser.getCreaturesMass()->getLastValue());
    dataAnalyser.getCreaturesWastedEnergy()->addValue(dataAnalyser.getCreaturesWastedEnergy()->getLastValue());
    dataAnalyser.getHeatEnergy()->addValue(dataAnalyser.getHeatEnergy()->getLastValue());
    dataAnalyser.getGroundEnergy()->addValue(dataAnalyser.getGroundEnergy()->getLastValue());
    dataAnalyser.getAverageInputNeurons()->addValue(dataAnalyser.getAverageInputNeurons()->getLastValue());
    dataAnalyser.getAverageOutputNeurons()->addValue(dataAnalyser.getAverageOutputNeurons()->getLastValue());
    dataAnalyser.getAverageLinks()->addValue(dataAnalyser.getAverageLinks()->getLastValue());
    dataAnalyser.getTotalEnergy()->addValue(dataAnalyser.getTotalEnergy()->getLastValue());
    dataAnalyser.getBrainOutputsTime()->addValue(dataAnalyser.getBrainOutputsTime()->getLastValue());
    dataAnalyser.getPrepareActionsTime()->addValue(dataAnalyser.getPrepareActionsTime()->getLastValue());
    dataAnalyser.getZero()->addValue(dataAnalyser.getZero()->getLastValue());




    for (auto const& runner : dataAnalyser.getRunnersPopulation()) {
        runner->addValue(runner->getLastValue());
    }


    std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_time = end - start;
    double statisticsTime = elapsed_time.count();
    dataAnalyser.getStatisticsTime()->addValue(statisticsTime);

    double totalTime = 0.0;
    totalTime += dataAnalyser.getEntityGridTime()->getLastValue();
    totalTime += dataAnalyser.getBrainProcessingTime()->getLastValue();
    totalTime += dataAnalyser.getBrainOutputsTime()->getLastValue();
    totalTime += dataAnalyser.getPrepareActionsTime()->getLastValue();
    totalTime += dataAnalyser.getExecuteActionsTime()->getLastValue();
    totalTime += dataAnalyser.getMoveCreaturesTime()->getLastValue();
    totalTime += dataAnalyser.getPopulationControlTime()->getLastValue();
//    totalTime += dataAnalyser.getVegetalisationTime()->getLastValue();
    totalTime += dataAnalyser.getCreatureSortingTime()->getLastValue();
    totalTime += dataAnalyser.getATickHavePassedTime()->getLastValue();

    totalTime += statisticsTime;
    dataAnalyser.getTotalTime()->addValue(totalTime);
}


void Farm::statistics() {
    std::chrono::system_clock::time_point start = std::chrono::system_clock::now();

    if (tickCount % STATISTICS_RATE != 0 && tickCount != 1) {
        handleNoStatistics();

        return;
    }

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

    std::vector<Life *> lifes = getLifes();
    for (int it = 0; it < lifes.size(); it++) {
        Life * currentLife = lifes.at(it);
        totalCreaturesEnergy += currentLife->getEnergyCenter()->getAvailableEnergy();
        totalCreaturesMass += currentLife->getEntity()->getMass();
        totalCreaturesWasted += currentLife->getEnergyCenter()->getWastedEnergy();

        totalInputNeurons += currentLife->getBrain()->getNeurons().size();
        totalOutputNeurons += currentLife->getBrain()->getOutputNeurons().size();
        totalLinks += currentLife->getBrain()->getLinks().size();
    }


    double totalHeat = 0.0;
    double totalTmpHeat = 0.0;
    double totalGround = 0.0;
    for (int it = 0; it < TILE_COUNT_WIDTH; it++) {
        for (int jt = 0; jt < TILE_COUNT_HEIGHT; jt++) {
            totalHeat += map->getTileAt(it, jt)->getHeat();
            totalGround += map->getTileAt(it, jt)->getGround();
            totalTmpHeat += map->getTileAt(it, jt)->getTmpHeat();

            std::vector<Entity *> entities = map->getTileAt(it, jt)->getEntities();

            for (int it = 0; it < entities.size(); it++) {
                Entity * entity = entities.at(it);
                totalFoodsEnergy += entity->getMass();
            }
        }
    }


    int totalEnergy = availableEnergy + totalFoodsEnergy + totalCreaturesEnergy + totalCreaturesMass + totalCreaturesWasted + totalHeat + totalGround + totalTmpHeat;
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

    for (int it = 0; it < CONCURRENT_LIFE_RUNNER; it++) {
        dataAnalyser.getRunnersPopulation().at(it)->addValue(lifesRunners.at(it)->getLifeCount());
    }



    double totalTime = 0.0;
    totalTime += dataAnalyser.getEntityGridTime()->getLastValue();
    totalTime += dataAnalyser.getBrainProcessingTime()->getLastValue();
    totalTime += dataAnalyser.getBrainOutputsTime()->getLastValue();
    totalTime += dataAnalyser.getPrepareActionsTime()->getLastValue();
    totalTime += dataAnalyser.getExecuteActionsTime()->getLastValue();
    totalTime += dataAnalyser.getMoveCreaturesTime()->getLastValue();
    totalTime += dataAnalyser.getPopulationControlTime()->getLastValue();
//    totalTime += dataAnalyser.getVegetalisationTime()->getLastValue();
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

    std::vector<Life *> lifes = getLifes();

    std::vector<std::vector<Entity *>> lifeEntityGrid;
    for (int it = 0; it < TILE_COUNT_WIDTH; it++) {
        for (int jt = 0; jt < TILE_COUNT_HEIGHT; jt++) {
            entityGrid.at(it).at(jt).clear();
        }
    }

    for (int it = 0; it < lifes.size(); it++) {
        Point simpleCoordinates = lifes.at(it)->getEntity()->getTileCoordinates();
        try {
            entityGrid.at(simpleCoordinates.getX()).at(simpleCoordinates.getY()).push_back(lifes.at(it)->getEntity());
        } catch (const std::exception&) {
            std::cout << "Error for simple coordinates: " << simpleCoordinates.getX() << " - " << simpleCoordinates.getY() << std::endl;
        }
    }


    std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_time = end - start;
    dataAnalyser.getEntityGridTime()->addValue(elapsed_time.count());
}

void Farm::removeDeadLifes() {
    for (int it = 0; it < lifesRunners.size(); it++) {
        lifesRunners.at(it)->removeDeadLifes();
    }
}


std::vector<Entity *> Farm::getAccessibleEntities(std::vector<Point> selectedChunks) {
    std::vector<Entity *> accessibleEntities;
    for (int jt = 0; jt < selectedChunks.size(); jt++) {
        Point currentChunk = selectedChunks.at(jt);

        std::vector<Entity *> tileEntities = map->getTileAt(currentChunk.getX(), currentChunk.getY())->getEntities();
        std::vector<Entity *> tileLifeEntity = entityGrid.at(currentChunk.getX()).at(currentChunk.getY());

        accessibleEntities.insert(accessibleEntities.end(), tileEntities.begin(), tileEntities.end());
        accessibleEntities.insert(accessibleEntities.end(), tileLifeEntity.begin(), tileLifeEntity.end());
    }
    return accessibleEntities;

}
std::vector<Tile *> Farm::getAccessibleTiles(Life * life, std::vector<Point> selectedChunks) {
    std::vector<Tile *> accessibleTiles;

    Point performerPoint = life->getEntity()->getPosition();
    Point tilePoint = performerPoint.getTileCoordinates();
    accessibleTiles.emplace_back(map->getTileAt(tilePoint.getX(), tilePoint.getY()));

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

    std::vector<Life *> tmpLifes = getLifes();

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
    std::vector<Life *> lifes = getLifes();
    for (int it = 0; it < lifes.size(); it++) {
        if (lifes.at(it)->getEntity()->getId() == id) {
            return lifes.at(it);
        }
    }

    return nullptr;
}

Entity * Farm::getEntityFromId(int id) {
    Life * foundLife = getLifeFromId(id);
    if (foundLife != nullptr) {
        return foundLife->getEntity();
    }

    std::vector<Entity *> allEntities = getEntities();

    for (int it = 0; it < allEntities.size(); it++) {
        if (allEntities.at(it)->getId() == id) {
            return allEntities.at(it);
        }
    }


    return nullptr;
}






CreatureNursery *Farm::getNursery() const {
    return nursery;
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


Map *Farm::getMap() const {
    return map;
}

vector<Life *> Farm::getLifes() {
    std::vector<Life *> lifes;
    for (auto const& lifesRunner: lifesRunners) {
        std::vector<Life *> returnedLifes = lifesRunner->getLifes();
        lifes.insert(lifes.end(), returnedLifes.begin(), returnedLifes.end());
    }
    return lifes;
}

vector<Entity *> Farm::getEntities() {
    std::vector<Entity *> allEntities;

    for (int it = 0; it < TILE_COUNT_WIDTH; it++) {
        for (int jt = 0; jt < TILE_COUNT_HEIGHT; jt++) {
            std::vector<Entity *> tileEntities = map->getTileAt(it, jt)->getEntities();
            if (tileEntities.size() == 0){
                continue;
            }

            allEntities.insert(allEntities.end(), tileEntities.begin(), tileEntities.end());
        }
    }

    return allEntities;
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



vector<Entity *> Farm::getAndClearEntitiesToAdd() {
    std::lock_guard<std::mutex> guard(add_mutex);

    vector<Entity *> currentEntitiesToAdd;
    currentEntitiesToAdd.insert(currentEntitiesToAdd.begin(), entityAdded.begin(), entityAdded.end());
    entityAdded.clear();

    return currentEntitiesToAdd;
}

vector<Life *> Farm::getAndClearLifesToAdd() {
    std::lock_guard<std::mutex> guard(add_mutex);

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

LifesRunner * Farm::getLessLoadedRunner() {
    LifesRunner * smallestLifeRunner = lifesRunners.at(0);
    int min = smallestLifeRunner->getLifeCount();

    for (auto const& currentRunner: lifesRunners) {
        if (currentRunner->getLifeCount() < min) {
            smallestLifeRunner = currentRunner;
            min = currentRunner->getLifeCount();
        }
    }
    return smallestLifeRunner;
}

void Farm::checkAndHandleLifeDying(Life * life) {
    if (life->getEnergyCenter()->getAvailableEnergy() <= 0) {
        Point performerPoint = life->getEntity()->getPosition();

        generateEntities(performerPoint, life->getEntity()->getColor(), 0.3f, 4000, life->getEntity()->getMass(), life->getEntity()->getSize());
        generateEntities(performerPoint, 0.04f, 0.2f, (0.1 * life->getEntity()->getSize() * MASS_TO_SIZE_RATIO), life->getEnergyCenter()->getWastedEnergy(), life->getEntity()->getSize());


        life->getEntity()->setMass(0.0);
        life->getEnergyCenter()->setWastedEnergy(0.0);
    }
}

FarmControl *Farm::getFarmControl() const {
    return farmControl;
}

void Farm::setFarmControl(FarmControl *farmControl) {
    Farm::farmControl = farmControl;
}
