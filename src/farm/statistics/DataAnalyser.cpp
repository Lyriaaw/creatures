//
// Created by Amalric Lombard de Buffières on 8/8/20.
//

#include <string>
#include "DataAnalyser.h"

DataAnalyser::DataAnalyser() {

    tickTime = new DataItem("tickTime", true);
    tickPerSecond = new DataItem("tickPerSecond", true);

    entityGridTime = new DataItem("entityGridTime", true);
    brainOutputsTime = new DataItem("brainOutputsTime", true);
    moveCreaturesTime = new DataItem("moveCreaturesTime", true);
    prepareActionsTime = new DataItem("prepareActionsTime", true);
    executeActionsTime = new DataItem("executeActionsTime", true);
    populationControlTime = new DataItem("populationControlTime", true);
    vegetalisationTime = new DataItem("vegetalisationTime", true);
    brainProcessingTime = new DataItem("brainProcessingTime", true);
    statisticsTime = new DataItem("statisticsTime", true);

    totalTime = new DataItem("totalTime", true);

    population = new DataItem("population", true);
    animals = new DataItem("population", true);
    vegetals = new DataItem("vegetals", true);
    naturalMatings = new DataItem("naturalMatings", true);

    totalEnergy = new DataItem("totalEnergy", true);
    availableEnergy = new DataItem("availableEnergy", true);
    foodEnergy = new DataItem("foodEnergy", true);
    creaturesEnergy = new DataItem("creaturesEnergy", true);
    heatEnergy = new DataItem("heatEnergy", true);
    groundEnergy = new DataItem("groundEnergy", true);
    creaturesMass = new DataItem("creaturesMass", true);
    energyToAdd = new DataItem("energyToAdd", true);

    averageScore = new DataItem("averageScore", true);
    bestScore = new DataItem("bestScore", true);
    medianScore = new DataItem("medianScore", true);
    firstQuartileScore = new DataItem("firstQuartileScore", true);
    lastQuartileScore = new DataItem("lastQuartileScore", true);

    chunkSelection = new DataItem("chunkSelection", true);
    sensorProcessing = new DataItem("sensorProcessing", true);
    brainProcessing = new DataItem("brainProcessing", true);
    totalGridGeneration = new DataItem("totalGridGeneration", true);
    externalActions = new DataItem("externalActions", true);


    totalActions = new DataItem("totalActions", true);
    captureGroundActions = new DataItem("captureGroundActions", true);
    captureHeatActions = new DataItem("captureHeatActions", true);
    duplicateActions = new DataItem("duplicateActions", true);
    mateActions = new DataItem("mateActions", true);
    eatActions = new DataItem("eatActions", true);

}







DataItem *DataAnalyser::getTickTime() const {
    return tickTime;
}


DataItem *DataAnalyser::getEntityGridTime() const {
    return entityGridTime;
}

DataItem *DataAnalyser::getBrainOutputsTime() const {
    return brainOutputsTime;
}

DataItem *DataAnalyser::getMoveCreaturesTime() const {
    return moveCreaturesTime;
}

DataItem *DataAnalyser::getPrepareActionsTime() const {
    return prepareActionsTime;
}

DataItem *DataAnalyser::getExecuteActionsTime() const {
    return executeActionsTime;
}

DataItem *DataAnalyser::getPopulationControlTime() const {
    return populationControlTime;
}

DataItem *DataAnalyser::getVegetalisationTime() const {
    return vegetalisationTime;
}

DataItem *DataAnalyser::getBrainProcessingTime() const {
    return brainProcessingTime;
}

DataItem *DataAnalyser::getPopulation() const {
    return population;
}

DataItem *DataAnalyser::getTotalEnergy() const {
    return totalEnergy;
}

DataItem *DataAnalyser::getAvailableEnergy() const {
    return availableEnergy;
}

DataItem *DataAnalyser::getFoodEnergy() const {
    return foodEnergy;
}

DataItem *DataAnalyser::getCreaturesEnergy() const {
    return creaturesEnergy;
}

DataItem *DataAnalyser::getTickPerSecond() const {
    return tickPerSecond;
}

DataItem *DataAnalyser::getAverageScore() const {
    return averageScore;
}

DataItem *DataAnalyser::getBestScore() const {
    return bestScore;
}

DataItem *DataAnalyser::getMedianScore() const {
    return medianScore;
}

DataItem *DataAnalyser::getFirstQuartileScore() const {
    return firstQuartileScore;
}

DataItem *DataAnalyser::getLastQuartileScore() const {
    return lastQuartileScore;
}

DataItem *DataAnalyser::getTotalTime() const {
    return totalTime;
}

DataItem *DataAnalyser::getStatisticsTime() const {
    return statisticsTime;
}

DataItem *DataAnalyser::getHeatEnergy() const {
    return heatEnergy;
}

DataItem *DataAnalyser::getGroundEnergy() const {
    return groundEnergy;
}

DataItem *DataAnalyser::getNaturalMatings() const {
    return naturalMatings;
}

DataItem *DataAnalyser::getChunkSelection() const {
    return chunkSelection;
}

DataItem *DataAnalyser::getSensorProcessing() const {
    return sensorProcessing;
}

DataItem *DataAnalyser::getBrainProcessing() const {
    return brainProcessing;
}

DataItem *DataAnalyser::getExternalActions() const {
    return externalActions;
}

DataItem *DataAnalyser::getTotalGridGeneration() const {
    return totalGridGeneration;
}

DataItem *DataAnalyser::getCreaturesMass() const {
    return creaturesMass;
}

DataItem *DataAnalyser::getTotalActions() const {
    return totalActions;
}

DataItem *DataAnalyser::getCaptureGroundActions() const {
    return captureGroundActions;
}

DataItem *DataAnalyser::getCaptureHeatActions() const {
    return captureHeatActions;
}

DataItem *DataAnalyser::getDuplicateActions() const {
    return duplicateActions;
}

DataItem *DataAnalyser::getMateActions() const {
    return mateActions;
}

DataItem *DataAnalyser::getEatActions() const {
    return eatActions;
}

DataItem *DataAnalyser::getEnergyToAdd() const {
    return energyToAdd;
}

DataItem *DataAnalyser::getAnimals() const {
    return animals;
}

DataItem *DataAnalyser::getVegetals() const {
    return vegetals;
}
