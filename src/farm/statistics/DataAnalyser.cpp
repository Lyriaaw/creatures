//
// Created by Amalric Lombard de Buffières on 8/8/20.
//

#include <string>
#include "DataAnalyser.h"
#include "../../World.h"

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
    creatureSortingTime = new DataItem("creatureSortingTime", true);
    aTickHavePassedTime = new DataItem("aTickHavePassedTime", true);

    totalTime = new DataItem("totalTime", true);

    population = new DataItem("population", true);
    naturalMatings = new DataItem("naturalMatings", true);

    totalEnergy = new DataItem("totalEnergy", true);
    availableEnergy = new DataItem("availableEnergy", true);
    foodEnergy = new DataItem("foodEnergy", true);
    creaturesEnergy = new DataItem("creaturesEnergy", true);
    creaturesMass = new DataItem("creaturesMass", true);
    creaturesWastedEnergy = new DataItem("creaturesWastedEnergy", true);
    heatEnergy = new DataItem("heatEnergy", true);
    groundEnergy = new DataItem("groundEnergy", true);

    averageScore = new DataItem("averageScore", true);
    bestScore = new DataItem("bestScore", true);
    medianScore = new DataItem("medianScore", true);
    firstQuartileScore = new DataItem("firstQuartileScore", true);
    lastQuartileScore = new DataItem("lastQuartileScore", true);

    chunkSelection = new DataItem("chunkSelection", true);
    sensorProcessing = new DataItem("sensorProcessing", true);
    brainProcessing = new DataItem("brainProcessing", true);
    externalActions = new DataItem("externalActions", true);
    accessibleEntities = new DataItem("accessibleEntities", true);

    zero = new DataItem("zero", true);

    colors = new GeneGraphAnalyser("Colors");

    averageInputNeurons = new DataItem("averageInputNeurons", true);
    averageOutputNeurons = new DataItem("averageOutputNeurons", true);
    averageLinks = new DataItem("averageLinks", true);



    poopCount = new DataItem("poopCount", true);
    pheromoneCount = new DataItem("pheromoneCount", true);
    eatCount = new DataItem("eatCount", true);
    eatLifeCount = new DataItem("eatLifeCount", true);
    mateSuccessCount = new DataItem("mateSuccessCount", true);
    mateFailureCount = new DataItem("mateFailureCount", true);
    biteCount = new DataItem("biteCount", true);
    biteLifeCount = new DataItem("biteLifeCount", true);
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

DataItem *DataAnalyser::getCreaturesMass() const {
    return creaturesMass;
}

DataItem *DataAnalyser::getCreaturesWastedEnergy() const {
    return creaturesWastedEnergy;
}

GeneGraphAnalyser *DataAnalyser::getColors() const {
    return colors;
}

DataItem *DataAnalyser::getZero() const {
    return zero;
}

DataItem *DataAnalyser::getCreatureSortingTime() const {
    return creatureSortingTime;
}

DataItem *DataAnalyser::getATickHavePassedTime() const {
    return aTickHavePassedTime;
}

DataItem *DataAnalyser::getAverageInputNeurons() const {
    return averageInputNeurons;
}

DataItem *DataAnalyser::getAverageOutputNeurons() const {
    return averageOutputNeurons;
}

DataItem *DataAnalyser::getAverageLinks() const {
    return averageLinks;
}

DataItem *DataAnalyser::getAccessibleEntities() const {
    return accessibleEntities;
}

const std::vector<DataItem *> &DataAnalyser::getRunnersPopulation() const {
    return runnersPopulation;
}

DataItem *DataAnalyser::getPoopCount() const {
    return poopCount;
}

DataItem *DataAnalyser::getPheromoneCount() const {
    return pheromoneCount;
}

DataItem *DataAnalyser::getEatCount() const {
    return eatCount;
}

DataItem *DataAnalyser::getBiteCount() const {
    return biteCount;
}

DataItem *DataAnalyser::getMateFailureCount() const {
    return mateFailureCount;
}

DataItem *DataAnalyser::getMateSuccessCount() const {
    return mateSuccessCount;
}

DataItem *DataAnalyser::getBiteLifeCount() const {
    return biteLifeCount;
}

DataItem *DataAnalyser::getEatLifeCount() const {
    return eatLifeCount;
}
