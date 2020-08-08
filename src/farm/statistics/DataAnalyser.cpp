//
// Created by Amalric Lombard de Buffières on 8/8/20.
//

#include "DataAnalyser.h"

DataAnalyser::DataAnalyser() {
    tickTime = new DataItem(true);
    tickPerSecond = new DataItem(true);

    entityGridTime = new DataItem(true);
    brainOutputsTime = new DataItem(true);
    moveCreaturesTime = new DataItem(true);
    prepareActionsTime = new DataItem(true);
    executeActionsTime = new DataItem(true);
    populationControlTime = new DataItem(true);
    vegetalisationTime = new DataItem(true);
    brainProcessingTime = new DataItem(true);

    totalTime = new DataItem(true);

    population = new DataItem(true);

    totalEnergy = new DataItem(true);
    availableEnergy = new DataItem(true);
    foodEnergy = new DataItem(true);
    creaturesEnergy = new DataItem(true);

    averageScore = new DataItem(true);
    bestScore = new DataItem(true);
    medianScore = new DataItem(true);
    firstQuartileScore = new DataItem(true);
    lastQuartileScore = new DataItem(true);
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
