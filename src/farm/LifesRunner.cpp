//
// Created by Amalric Lombard de Buffières on 10/21/20.
//

#include "LifesRunner.h"
#include "../websockets/WebUiConnection.h"

LifesRunner::LifesRunner(int id): id(id), tick(0) {
    tickStart = std::chrono::system_clock::now();
    tickEnd = std::chrono::system_clock::now();
}

void LifesRunner::addLife(Life * life) {
    std::lock_guard<std::mutex> guard(lifes_mutex);

    lifes.emplace_back(life);
}


int LifesRunner::getLifeCount() {
    return lifes.size();
}

void LifesRunner::handleThread() {
    auto f = [&]() {
        while (farmControl->isRunning()) {
            if (!farmControl->isPaused()) {
                moveCreatures();
                brainProcessing(false);
                removeDeadLifes();
                tick++;

                tickEnd = std::chrono::system_clock::now();
                std::chrono::duration<double> elapsed_time = tickEnd - tickStart;
                tickStart = std::chrono::system_clock::now();
                double tickTime = elapsed_time.count();

                if (tick == 1) {
                    dataAnalyser.getTickTime()->addValue(0);
                } else {
                    dataAnalyser.getTickTime()->addValue(tickTime);
                }

                dataAnalyser.getTickPerSecond()->addValue(1.0 / tickTime);


            } else {
                usleep(500000);
            }

            tickStart = std::chrono::system_clock::now();

            if (tick % 20 == 0 && !farmControl->isPaused()) {
                triggerUpdate(id);
            }

            if (this->tick > medianTick) {
                double secondToSleep = (this->dataAnalyser.getTickTime()->getLastValue() / 10.0) * double(double(tick) - double(medianTick));
                usleep(secondToSleep * 1000000.0);
            }

        }
    };

    std::thread runnerThread(f);
    runnerThread.detach();
}

void LifesRunner::brainProcessing(bool paused) {
    std::chrono::system_clock::time_point start = std::chrono::system_clock::now();


    std::vector<Life *> currentLifes = lifes;

    std::chrono::system_clock::time_point chunkProcessingStart = std::chrono::system_clock::now();
    for (int it = 0; it < currentLifes.size(); it++) {
        Life *currentLife = currentLifes.at(it);
        if (!currentLife->isAlive()) {
            continue;
        }
        currentLife->processSelectedChunks();
    }
    std::chrono::system_clock::time_point chunkProcessingEnd = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_timeChunk = chunkProcessingEnd - chunkProcessingStart;
    dataAnalyser.getChunkSelection()->addValue(elapsed_timeChunk.count());



    std::chrono::system_clock::time_point sensorProcessingStart = std::chrono::system_clock::now();
    for (int it = 0; it < currentLifes.size(); it++) {
        Life *currentLife = currentLifes.at(it);
        if (!currentLife->isAlive()) {
            continue;
        }

        std::vector<Entity *> accessibleEntities = getAccessibleEntities(currentLife->getSelectedChunks());
        std::vector<Tile *> accessibleTiles = getAccessibleTiles(currentLife, currentLife->getSelectedChunks());
        currentLife->processSensors(accessibleEntities, accessibleTiles);
    }
    std::chrono::system_clock::time_point sensorProcessingEnd = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_timeSensor = sensorProcessingEnd - sensorProcessingStart;
    dataAnalyser.getSensorProcessing()->addValue(elapsed_timeSensor.count());




    std::chrono::system_clock::time_point brainProcessingStart = std::chrono::system_clock::now();

    for (int it = 0; it < currentLifes.size(); it++) {
        Life *currentLife = lifes.at(it);
        if (!currentLife->isAlive()) {
            continue;
        }

        currentLife->processBrain();
    }
    std::chrono::system_clock::time_point brainProcessingEnd = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_timeBrain = brainProcessingEnd - brainProcessingStart;
    dataAnalyser.getBrainProcessing()->addValue(elapsed_timeBrain.count());


    if (paused) {
        std::chrono::system_clock::time_point externalActionsStart = std::chrono::system_clock::now();
        std::chrono::system_clock::time_point externalActionsEnd = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_timeActions = externalActionsEnd - externalActionsStart;
        dataAnalyser.getExternalActions()->addValue(elapsed_timeActions.count());

        return;
    }




    executeCreatureActions();



}

void LifesRunner::executeCreatureActions() {
    std::chrono::system_clock::time_point externalActionsStart = std::chrono::system_clock::now();

    int poopCount = 0;
    int eatCount = 0;
    int biteCount = 0;
    int pheromoneCount = 0;
    int biteLifeCount = 0;
    int eatLifeCount = 0;
    int mateFailureCount = 0;
    int mateSuccessCount = 0;

    std::vector<Life *> currentLifes = lifes;

    for (int it = 0; it < currentLifes.size(); it++) {
        Life * life = currentLifes.at(it);

        if (life == nullptr) {
            continue;
        }

        std::vector<Point> selectedTiles = life->getSelectedChunks();
        std::vector<Entity *> accessibleEntities = getAccessibleEntities(selectedTiles);

        std::vector<ActionDTO> currentCreatureActions = life->executeExternalActions(accessibleEntities);


        for (auto const& action : currentCreatureActions) {
            if (action.getType() == "POOP") {
                poopCount++;
                handlePoop(life);
                continue;
            }

            if (action.getType() == "EAT_ENTITY") {

                for (int jt = 0; jt < accessibleEntities.size(); jt++) {
                    if (accessibleEntities.at(jt)->getId() == action.getSubjectId()) {
                        eatCount++;
                        handleEating(life, accessibleEntities.at(jt));
                        jt = accessibleEntities.size();
                    }
                }

                continue;
            }

            if (action.getType() == "BITE_ENTITY") {

                for (int jt = 0; jt < accessibleEntities.size(); jt++) {
                    if (accessibleEntities.at(jt)->getId() == action.getSubjectId()) {
                        biteCount++;
                        handleBiting(life, accessibleEntities.at(jt));
                        jt = accessibleEntities.size();
                    }
                }

                continue;
            }

            if (action.getType() == "EMIT_PHEROMONE") {
//            handlePoop(performer);
                handlePheromoneEmission(life, action);
                pheromoneCount++;
                continue;
            }

            if (action.getType() == "BITE_LIFE") {
                handleBitingLife(life, action);
                biteLifeCount++;
                continue;
            }


            if (action.getType() == "EAT_LIFE") {
                handleEatLife(life, action);
                eatLifeCount++;
                continue;

            }

            if (action.getType() == "MATE") {
                bool success = handleMating(life, action);

                if (success) {
                    mateSuccessCount++;
                } else {
                    mateFailureCount++;
                }

                continue;
            }

        }


    }



    dataAnalyser.getPoopCount()->addValue(poopCount);
    dataAnalyser.getEatCount()->addValue(eatCount);
    dataAnalyser.getBiteCount()->addValue(biteCount);
    dataAnalyser.getPheromoneCount()->addValue(pheromoneCount);
    dataAnalyser.getBiteLifeCount()->addValue(biteLifeCount);
    dataAnalyser.getEatLifeCount()->addValue(eatLifeCount);

    dataAnalyser.getMateFailureCount()->addValue(mateFailureCount);
    dataAnalyser.getMateSuccessCount()->addValue(mateSuccessCount);

    std::chrono::system_clock::time_point externalActionsEnd = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_timeActions = externalActionsEnd - externalActionsStart;
    dataAnalyser.getExternalActions()->addValue(elapsed_timeActions.count());
}

void LifesRunner::moveCreatures() {
    std::chrono::system_clock::time_point start = std::chrono::system_clock::now();

    std::vector<Life *> currentLifes = lifes;


    std::vector<Entity* > newEntities;
    for (int it = 0; it < currentLifes.size(); it++) {
        Life *currentLife = currentLifes.at(it);
        Point tilePoint = currentLife->getEntity()->getTileCoordinates();

        std::vector<Entity* > producedEntities = currentLife->executeInternalActions();
//        newEntities.insert(newEntities.begin(), producedEntities.begin(), producedEntities.end());

        double releasedHeat = 0.0;
        if (tick % 10 == 0) {
            releasedHeat = currentLife->giveawayEnergy();
            map->getTileAt(tilePoint.getX(), tilePoint.getY())->addTmpHeat(releasedHeat);
        }



        checkAndHandleLifeDying(currentLife);
    }

    removeDeadLifes();

    std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_time = end - start;
    dataAnalyser.getMoveCreatures()->addValue(elapsed_time.count());
}


std::vector<Life *> LifesRunner::removeDeadLifes() {
    std::vector<Life *> newLifes;
    std::vector<Life *> deletedLifes;
    for (int it = 0; it < lifes.size(); it++) {

        if (lifes.at(it)->isAlive()) {
            newLifes.emplace_back(lifes.at(it));
        } else {
            deletedLifes.emplace_back(lifes.at(it));
        }
    }

    lifes.clear();
    lifes = newLifes;

    return deletedLifes;
}

void LifesRunner::checkAndHandleLifeDying(Life * life) {
    if (life->getEnergyCenter()->getAvailableEnergy() <= 0) {
        Point performerPoint = life->getEntity()->getPosition();

        generateEntities(performerPoint, life->getEntity()->getColor(), 0.3f, 4000, life->getEntity()->getMass(), life->getEntity()->getSize());
        generateEntities(performerPoint, 0.04f, 0.2f, (0.1 * life->getEntity()->getSize() * MASS_TO_SIZE_RATIO), life->getEnergyCenter()->getWastedEnergy(), life->getEntity()->getSize());


        life->getEntity()->setMass(0.0);
        life->getEnergyCenter()->setWastedEnergy(0.0);
    }
}






// Actions


void LifesRunner::handlePoop(Life * subject) {
    if (!subject->getEntity()->tryLockInteraction()) {
        return;
    }

    if (subject->getEnergyCenter()->getWastedEnergy() == 0) {
        subject->getEntity()->unlockInteraction();
        return;
    }

    Point position = subject->getEntity()->getPosition();

    double maxPoopSize = 5;

    double poopedEnergy = subject->getEnergyCenter()->getWastedEnergy();
    generateEntities(position, 0.04f, 0.2f, (0.1 * subject->getEntity()->getSize() * MASS_TO_SIZE_RATIO), poopedEnergy, subject->getEntity()->getSize());


    subject->getEnergyCenter()->setWastedEnergy(0.0);

    ActionDTO executedAction = ActionDTO(0, 0, "POOP");
    executedAction.setTilePosition(position.getTileCoordinates());
    recordExecutedAction(executedAction);
    subject->getEntity()->unlockInteraction();
}

void LifesRunner::handleEating(Life * performer, Entity * subject) {
    if (!performer->getEntity()->tryLockInteraction()) {
        return;
    }

    if (!subject->tryLockInteraction()) {
        performer->getEntity()->unlockInteraction();
        return;
    }

    if (subject->getMass() == 0) {
        performer->getEntity()->unlockInteraction();
        subject->unlockInteraction();
        return;
    }

    performer->addEnergy(subject->getMass());
    subject->setMass(0.0);

    subject->unlockInteraction();
    performer->getEntity()->unlockInteraction();


    Point performerPoint = performer->getEntity()->getPosition();
    Point tilePoint = performerPoint.getTileCoordinates();
    ActionDTO executedAction = ActionDTO(0, 0, "EAT_ENTITY");
    executedAction.setTick(tick);
    executedAction.setTilePosition(tilePoint);
    recordExecutedAction(executedAction);


}

void LifesRunner::handleBiting(Life * performer, Entity * subject) {
    if (!performer->getEntity()->tryLockInteraction()) {
        return;
    }


    if (!subject->tryLockInteraction()) {
        performer->getEntity()->unlockInteraction();
        return;
    }

    Point performerPoint = performer->getEntity()->getPosition();
    Point tilePoint = performerPoint.getTileCoordinates();

    generateEntities(subject->getPosition(), subject->getColor(), subject->getBrightness(), (subject->getSize() * MASS_TO_SIZE_RATIO) / 2.0, subject->getMass(), subject->getSize());
    subject->setMass(0.0);

    subject->unlockInteraction();
    performer->getEntity()->unlockInteraction();



    ActionDTO executedAction = ActionDTO(0, 0, "BITE_ENTITY");
    executedAction.setTilePosition(tilePoint);
    executedAction.setTick(tick);
    recordExecutedAction(executedAction);
}

void LifesRunner::handlePheromoneEmission(Life * performer, ActionDTO action) {
    if (!performer->getEntity()->tryLockInteraction()) {
        return;
    }

    Point performerPoint = performer->getEntity()->getPosition();
    Point tilePoint = performerPoint.getTileCoordinates();

    map->getTileAt(tilePoint.getX(), tilePoint.getY())->addPheromone(action.getPheromoneEmissionColor(), performer->getEntity()->getSize());

    performer->getEntity()->unlockInteraction();

    ActionDTO executedAction = ActionDTO(0, 0, "EMIT_PHEROMONE");
    executedAction.setTilePosition(tilePoint);
    executedAction.setTick(tick);
    recordExecutedAction(executedAction);
}

void LifesRunner::handleBitingLife(Life * performer, ActionDTO action) {
    if (!performer->getEntity()->tryLockInteraction()) {
        return;
    }

    Life * foundLife = getLifeFromId(action.getSubjectId());
    if (foundLife == nullptr) {
        performer->getEntity()->unlockInteraction();
        return;
    }

    if (!foundLife->getEntity()->tryLockInteraction()) {
        performer->getEntity()->unlockInteraction();
        return;
    }


    double mouthSize = performer->getEntity()->getSize() / 3.0;

    Point performerPoint = performer->getEntity()->getPosition();
    Point tilePoint = performerPoint.getTileCoordinates();
    Tile * tile = map->getTileAt(tilePoint.getX(), tilePoint.getY());

    double takenEnergy = std::min(mouthSize * 10.0, foundLife->getEnergyCenter()->getAvailableEnergy());
    foundLife->getEnergyCenter()->removeAvailableEnergy(takenEnergy);
    tile->addTmpHeat(takenEnergy);

    checkAndHandleLifeDying(foundLife);
    performer->getEntity()->unlockInteraction();
    foundLife->getEntity()->unlockInteraction();

    ActionDTO executedAction = ActionDTO(0, 0, "BITE_LIFE");
    executedAction.setTilePosition(tilePoint);
    executedAction.setTick(tick);
    recordExecutedAction(executedAction);

}

void LifesRunner::handleEatLife(Life * performer, ActionDTO action) {
    if (!performer->getEntity()->tryLockInteraction()) {
        return;
    }

    Life * foundLife = getLifeFromId(action.getSubjectId());
    if (foundLife == nullptr) {
        performer->getEntity()->unlockInteraction();
        return;
    }

    if (!foundLife->getEntity()->tryLockInteraction()) {
        performer->getEntity()->unlockInteraction();
        return;
    }


    double wastedEnergy = performer->addEnergy(foundLife->getEntity()->getMass());
    foundLife->getEntity()->setMass(0.0);

    Point performerPoint = performer->getEntity()->getPosition();
    Point tilePoint = performerPoint.getTileCoordinates();

    map->getTileAt(tilePoint.getX(), tilePoint.getY())->addGround(wastedEnergy);

    Tile * tile = map->getTileAt(tilePoint.getX(), tilePoint.getY());

    tile->addGround(foundLife->getEnergyCenter()->getWastedEnergy());
    tile->addHeat(foundLife->getEnergyCenter()->getAvailableEnergy());

    foundLife->getEnergyCenter()->setAvailableEnergy(0.0);
    foundLife->getEnergyCenter()->setWastedEnergy(0.0);

    checkAndHandleLifeDying(foundLife);



    performer->getEntity()->unlockInteraction();
    foundLife->getEntity()->unlockInteraction();

    ActionDTO executedAction = ActionDTO(0, 0, "EAT_LIFE");
    executedAction.setTilePosition(tilePoint);
    executedAction.setTick(tick);
    recordExecutedAction(executedAction);

}

bool LifesRunner::handleMating(Life * performer, ActionDTO action) {
    if (!performer->getEntity()->tryLockInteraction()) {
        return false;
    }

    Life * foundLife = getLifeFromId(action.getSubjectId());
    if (foundLife == nullptr) {
        performer->getEntity()->unlockInteraction();
        return false;
    }

    if (!foundLife->getEntity()->tryLockInteraction()) {
        performer->getEntity()->unlockInteraction();
        return false;
    }



    bool fatherCanReproduce = performer->getEntity()->getMass() > performer->getEnergyCenter()->getMaxMass() / 3.f && performer->getEntity()->getAge() > 10;
    bool motherCanReproduce = foundLife->getEntity()->getMass() > foundLife->getEnergyCenter()->getMaxMass() / 3.f && foundLife->getEntity()->getAge() > 10;

    if (!fatherCanReproduce || !motherCanReproduce) {
        performer->getEntity()->unlockInteraction();
        foundLife->getEntity()->unlockInteraction();
        recordMatingFailure(performer);
        return false;
    }
//
    Life * child = this->creatureNursery->Mate(performer, foundLife);

    double givenEnergyToChildGoal = child->getEnergyCenter()->getMaxMass() / 2.f;

    double givenFatherEnergy = std::min(performer->getEntity()->getMass() / 4.0, givenEnergyToChildGoal / 2.0);
    double givenMotherEnergy = std::min(foundLife->getEntity()->getMass() / 4.0, givenEnergyToChildGoal / 2.0);

    double actualGivenFatherEnergy = performer->getEntity()->removeMass(givenFatherEnergy);
    double actualGivenMotherEnergy = foundLife->getEntity()->removeMass(givenMotherEnergy);

    if (givenFatherEnergy != actualGivenFatherEnergy || givenMotherEnergy != actualGivenMotherEnergy) {
        std::cout << "Wrong energy given" << std::endl;
    }


    double totalGivenEnergy = actualGivenFatherEnergy + actualGivenMotherEnergy;

    checkAndHandleLifeDying(performer);
    checkAndHandleLifeDying(foundLife);

    performer->getEntity()->unlockInteraction();
    foundLife->getEntity()->unlockInteraction();

    Point childCoordinate = child->getEntity()->getPosition();
    Point tileChildPosition = childCoordinate.getTileCoordinates();

    if (totalGivenEnergy > givenEnergyToChildGoal / 2.0) {
        child->getEntity()->setMass(totalGivenEnergy / 2.0);
        child->getEnergyCenter()->setAvailableEnergy(totalGivenEnergy / 2.0);
        addLifeToFarm(child);


        ActionDTO executedAction = ActionDTO(0, 0, "MATE");
        executedAction.setTilePosition(tileChildPosition);
        executedAction.setTick(tick);
        recordExecutedAction(executedAction);

        return true;
    }



    map->getTileAt(tileChildPosition.getX(), tileChildPosition.getY())->addGround(totalGivenEnergy);
    recordMatingFailure(performer);

    return false;
}

void LifesRunner::recordMatingFailure(Life * performer) {
    Point point = performer->getEntity()->getTileCoordinates();

    ActionDTO executedAction = ActionDTO(0, 0, "MATE");
    executedAction.setTilePosition(point);
    executedAction.setTick(tick);
    recordExecutedAction(executedAction);
}














// Getters and setters

void LifesRunner::setGenerateEntities(
        const std::function<void(Point, float, float, double, double, double)> &generateEntities) {
    LifesRunner::generateEntities = generateEntities;
}

const std::vector<ActionDTO> &LifesRunner::getActions() const {
    return actions;
}

void LifesRunner::clearActions() {
    actions.clear();
}

void LifesRunner::setGetAccessibleEntities(
        const std::function<std::vector<Entity *>(std::vector<Point>)> &getAccessibleEntities) {
    LifesRunner::getAccessibleEntities = getAccessibleEntities;
}

void LifesRunner::setGetAccessibleTiles(
        const std::function<std::vector<Tile *>(Life *, std::vector<Point>)> &getAccessibleTiles) {
    LifesRunner::getAccessibleTiles = getAccessibleTiles;
}

void LifesRunner::setMap(Map *map) {
    LifesRunner::map = map;
}

std::vector<Life *> LifesRunner::getLifes(){
    std::lock_guard<std::mutex> guard(lifes_mutex);
    std::vector<Life *> tmpLifes = lifes;
    return tmpLifes;
}

const LifeRunnerDataTracker &LifesRunner::getDataAnalyser() const {
    return dataAnalyser;
}

void LifesRunner::setRecordExecutedAction(const std::function<void(ActionDTO)> &recordExecutedAction) {
    LifesRunner::recordExecutedAction = recordExecutedAction;
}

int LifesRunner::getTick() const {
    return tick;
}

void LifesRunner::setTick(int tick) {
    LifesRunner::tick = tick;
}

void LifesRunner::setGetLifeFromId(const std::function<Life *(int)> &getLifeFromId) {
    LifesRunner::getLifeFromId = getLifeFromId;
}

void LifesRunner::setAddLifeToFarm(const std::function<void(Life *)> &addLifeToFarm) {
    LifesRunner::addLifeToFarm = addLifeToFarm;
}

void LifesRunner::setCreatureNursery(CreatureNursery *creatureNursery) {
    LifesRunner::creatureNursery = creatureNursery;
}







json LifesRunner::asJson() {
    json runner;
    runner["id"] = id;
    runner["tick"] = tick;
    runner["creatures_count"] = this->lifes.size();

    json times;
    times["tickTime"] = this->dataAnalyser.getTickTime()->getAveragedLastValue();
    times["tickPerSecond"] = this->dataAnalyser.getTickPerSecond()->getAveragedLastValue();
    times["poopCount"] = this->dataAnalyser.getPoopCount()->getAveragedLastValue();
    times["pheromoneCount"] = this->dataAnalyser.getPheromoneCount()->getAveragedLastValue();
    times["eatCount"] = this->dataAnalyser.getEatCount()->getAveragedLastValue();
    times["eatLifeCount"] = this->dataAnalyser.getEatLifeCount()->getAveragedLastValue();
    times["mateSuccessCount"] = this->dataAnalyser.getMateSuccessCount()->getAveragedLastValue();
    times["mateFailureCount"] = this->dataAnalyser.getMateFailureCount()->getAveragedLastValue();
    times["biteCount"] = this->dataAnalyser.getBiteCount()->getAveragedLastValue();
    times["biteLifeCount"] = this->dataAnalyser.getBiteLifeCount()->getAveragedLastValue();
    times["chunkSelection"] = this->dataAnalyser.getChunkSelection()->getAveragedLastValue();
    times["sensorProcessing"] = this->dataAnalyser.getSensorProcessing()->getAveragedLastValue();
    times["brainProcessing"] = this->dataAnalyser.getBrainProcessing()->getAveragedLastValue();
    times["externalActions"] = this->dataAnalyser.getExternalActions()->getAveragedLastValue();
    times["moveCreatures"] = this->dataAnalyser.getMoveCreatures()->getAveragedLastValue();

    runner["times"] = times;


    return runner;
}

void LifesRunner::setFarmControl(FarmControl *farmControl) {
    LifesRunner::farmControl = farmControl;
}

int LifesRunner::getId() const {
    return id;
}

void LifesRunner::setId(int id) {
    LifesRunner::id = id;
}

void LifesRunner::setTriggerUpdate(const std::function<void(int)> &triggerUpdate) {
    LifesRunner::triggerUpdate = triggerUpdate;
}

void LifesRunner::setMedianTick(int medianTick) {
    LifesRunner::medianTick = medianTick;
}
