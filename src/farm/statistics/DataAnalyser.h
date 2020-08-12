//
// Created by Amalric Lombard de Buffières on 8/8/20.
//

#ifndef CREATURES_DATAANALYSER_H
#define CREATURES_DATAANALYSER_H


#include "DataItem.h"

class DataAnalyser {
private:
    DataItem * tickTime;
    DataItem * tickPerSecond;

    DataItem * averageScore;
    DataItem * bestScore;
    DataItem * medianScore;
    DataItem * firstQuartileScore;
    DataItem * lastQuartileScore;

    DataItem * entityGridTime;
    DataItem * brainOutputsTime;
    DataItem * moveCreaturesTime;
    DataItem * prepareActionsTime;
    DataItem * executeActionsTime;
    DataItem * populationControlTime;
    DataItem * vegetalisationTime;
    DataItem * statisticsTime;
    DataItem * brainProcessingTime;

    DataItem * chunkSelection;
    DataItem * totalGridGeneration;
    DataItem * sensorProcessing;
    DataItem * brainProcessing;
    DataItem * externalActions;

    DataItem * totalTime;

    DataItem * population;
    DataItem * naturalMatings;

    DataItem * totalEnergy;
    DataItem * availableEnergy;
    DataItem * foodEnergy;
    DataItem * creaturesEnergy;

    DataItem * heatEnergy;
    DataItem * groundEnergy;


public:
    DataAnalyser();

    DataItem *getTickTime() const;

    DataItem *getTickPerSecond() const;

    DataItem *getEntityGridTime() const;

    DataItem *getBrainOutputsTime() const;

    DataItem *getMoveCreaturesTime() const;

    DataItem *getPrepareActionsTime() const;

    DataItem *getExecuteActionsTime() const;

    DataItem *getPopulationControlTime() const;

    DataItem *getVegetalisationTime() const;

    DataItem *getBrainProcessingTime() const;

    DataItem *getPopulation() const;

    DataItem *getTotalEnergy() const;

    DataItem *getAvailableEnergy() const;

    DataItem *getFoodEnergy() const;

    DataItem *getCreaturesEnergy() const;

    DataItem *getAverageScore() const;

    DataItem *getBestScore() const;

    DataItem *getMedianScore() const;

    DataItem *getFirstQuartileScore() const;

    DataItem *getLastQuartileScore() const;

    DataItem *getTotalTime() const;

    DataItem *getStatisticsTime() const;

    DataItem *getHeatEnergy() const;

    DataItem *getGroundEnergy() const;

    DataItem *getNaturalMatings() const;

    DataItem *getChunkSelection() const;

    DataItem *getSensorProcessing() const;

    DataItem *getBrainProcessing() const;

    DataItem *getExternalActions() const;

    DataItem *getTotalGridGeneration() const;
};


#endif //CREATURES_DATAANALYSER_H
