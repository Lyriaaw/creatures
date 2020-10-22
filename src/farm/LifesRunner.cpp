//
// Created by Amalric Lombard de Buffières on 10/21/20.
//

#include "LifesRunner.h"

LifesRunner::LifesRunner() {}

void LifesRunner::addLife(Life * life) {
    std::lock_guard<std::mutex> guard(lifes_mutex);

    lifes.emplace_back(life);
}


int LifesRunner::getLifeCount() {
    return lifes.size();
}

void LifesRunner::brainProcessing(bool paused) {
    std::chrono::system_clock::time_point start = std::chrono::system_clock::now();

    actions.clear();

    std::chrono::system_clock::time_point chunkProcessingStart = std::chrono::system_clock::now();
    for (int it = 0; it < lifes.size(); it++) {
        Life *currentLife = lifes.at(it);
        if (!currentLife->isAlive()) {
            continue;
        }
        currentLife->processSelectedChunks();
    }
    std::chrono::system_clock::time_point chunkProcessingEnd = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_timeChunk = chunkProcessingEnd - chunkProcessingStart;
    dataAnalyser.getChunkSelection()->addValue(elapsed_timeChunk.count());



    std::chrono::system_clock::time_point sensorProcessingStart = std::chrono::system_clock::now();
    for (int it = 0; it < lifes.size(); it++) {
        Life *currentLife = lifes.at(it);
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

    for (int it = 0; it < lifes.size(); it++) {
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


        std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_time = end - start;
        dataAnalyser.getBrainProcessingTime()->addValue(elapsed_time.count());
        return;
    }

    int poopCount = 0;
    int eatCount = 0;
    std::chrono::system_clock::time_point externalActionsStart = std::chrono::system_clock::now();
    for (int it = 0; it < lifes.size(); it++) {
        Life *currentLife = lifes.at(it);
        if (!currentLife->isAlive()) {
            continue;
        }

        std::vector<Entity *> accessibleEntities = getAccessibleEntities(currentLife->getSelectedChunks());

        std::vector<ActionDTO> currentCreatureActions = currentLife->executeExternalActions(accessibleEntities);

        std::vector<ActionDTO> remainingActions;

        for (auto const& action : currentCreatureActions) {
            if (action.getType() == "POOP") {
                poopCount++;
                handlePoop(currentLife);
                continue;
            }

            if (action.getType() == "EAT_ENTITY") {

                for (int jt = 0; jt < accessibleEntities.size(); jt++) {
                    if (accessibleEntities.at(jt)->getId() == action.getSubjectId()) {
                        eatCount++;
                        handleEating(currentLife, accessibleEntities.at(jt));
                        jt = accessibleEntities.size();
                    }
                }

                continue;
            }



            remainingActions.emplace_back(action);
        }



        actions.insert(actions.end(), remainingActions.begin(), remainingActions.end());
    }
    std::chrono::system_clock::time_point externalActionsEnd = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_timeActions = externalActionsEnd - externalActionsStart;
    dataAnalyser.getExternalActions()->addValue(elapsed_timeActions.count());

    dataAnalyser.getPoopCount()->addValue(poopCount);
    dataAnalyser.getEatCount()->addValue(eatCount);


    std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_time = end - start;
    dataAnalyser.getBrainProcessingTime()->addValue(elapsed_time.count());
}

void LifesRunner::moveCreatures() {
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


    std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_time = end - start;
    dataAnalyser.getMoveCreaturesTime()->addValue(elapsed_time.count());
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

void LifesRunner::handlePoop(Life * subject) {
    if (subject->getEnergyCenter()->getWastedEnergy() == 0) {
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
}


void LifesRunner::handleEating(Life * performer, Entity * subject) {

    subject->tryLockInteraction();

    if (subject->getMass() == 0) {
        subject->unlockInteraction();
        return;
    }

    performer->addEnergy(subject->getMass());
    subject->setMass(0.0);

    subject->unlockInteraction();


    Point performerPoint = performer->getEntity()->getPosition();
    Point tilePoint = performerPoint.getTileCoordinates();
    ActionDTO executedAction = ActionDTO(0, 0, "EAT_ENTITY");
    executedAction.setTilePosition(tilePoint);
    recordExecutedAction(executedAction);

}













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

const std::vector<Life *> &LifesRunner::getLifes() const {
    return lifes;
}

const DataAnalyser &LifesRunner::getDataAnalyser() const {
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
