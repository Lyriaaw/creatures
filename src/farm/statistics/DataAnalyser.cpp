//
// Created by Amalric Lombard de Buffières on 8/8/20.
//

#include "DataAnalyser.h"

DataAnalyser::DataAnalyser() {
    tickTime = new DataItem();
    tickPerSecond = new DataItem();

    entityGridTime = new DataItem();
    brainOutputsTime = new DataItem();
    moveCreaturesTime = new DataItem();
    prepareActionsTime = new DataItem();
    executeActionsTime = new DataItem();
    populationControlTime = new DataItem();
    vegetalisationTime = new DataItem();
    brainProcessingTime = new DataItem();

    population = new DataItem();

    totalEnergy = new DataItem();
    availableEnergy = new DataItem();
    foodEnergy = new DataItem();
    creaturesEnergy = new DataItem();
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

