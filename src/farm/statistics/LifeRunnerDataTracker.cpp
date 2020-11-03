//
// Created by Amalric Lombard de Buffières on 10/25/20.
//

#include "LifeRunnerDataTracker.h"

LifeRunnerDataTracker::LifeRunnerDataTracker() {
    tickTime = new DataItem("tickTime", true);
    tickPerSecond = new DataItem("tickPerSecond", true);
    lifesCount = new DataItem("lifesCount", true);


    poopCount = new DataItem("poopCount", true);
    pheromoneCount = new DataItem("pheromoneCount", true);
    eatCount = new DataItem("eatCount", true);
    eatLifeCount = new DataItem("eatLifeCount", true);
    mateSuccessCount = new DataItem("mateSuccessCount", true);
    mateFailureCount = new DataItem("mateFailureCount", true);
    biteCount = new DataItem("biteCount", true);
    biteLifeCount = new DataItem("biteLifeCount", true);

    chunkSelection = new DataItem("chunkSelection", true);
    sensorProcessing = new DataItem("sensorProcessing", true);
    brainProcessing = new DataItem("brainProcessing", true);
    externalActions = new DataItem("externalActions", true);
    moveCreatures = new DataItem("moveCreatures", true);
}

DataItem *LifeRunnerDataTracker::getPoopCount() const {
    return poopCount;
}

DataItem *LifeRunnerDataTracker::getPheromoneCount() const {
    return pheromoneCount;
}

DataItem *LifeRunnerDataTracker::getEatCount() const {
    return eatCount;
}

DataItem *LifeRunnerDataTracker::getEatLifeCount() const {
    return eatLifeCount;
}

DataItem *LifeRunnerDataTracker::getMateFailureCount() const {
    return mateFailureCount;
}

DataItem *LifeRunnerDataTracker::getMateSuccessCount() const {
    return mateSuccessCount;
}

DataItem *LifeRunnerDataTracker::getBiteCount() const {
    return biteCount;
}

DataItem *LifeRunnerDataTracker::getBiteLifeCount() const {
    return biteLifeCount;
}

DataItem *LifeRunnerDataTracker::getChunkSelection() const {
    return chunkSelection;
}

DataItem *LifeRunnerDataTracker::getSensorProcessing() const {
    return sensorProcessing;
}

DataItem *LifeRunnerDataTracker::getBrainProcessing() const {
    return brainProcessing;
}

DataItem *LifeRunnerDataTracker::getExternalActions() const {
    return externalActions;
}

DataItem *LifeRunnerDataTracker::getTickTime() const {
    return tickTime;
}

DataItem *LifeRunnerDataTracker::getTickPerSecond() const {
    return tickPerSecond;
}

DataItem *LifeRunnerDataTracker::getMoveCreatures() const {
    return moveCreatures;
}

DataItem *LifeRunnerDataTracker::getLifesCount() const {
    return lifesCount;
}


