//
// Created by Amalric Lombard de Buffières on 8/8/20.
//

#ifndef CREATURES_DATAANALYSER_H
#define CREATURES_DATAANALYSER_H


#include "DataItem.h"

class DataAnalyser {
private:
    DataItem * tickTime;
    DataItem * betweenTickTime;

    DataItem * entityGridTime;
    DataItem * brainOutputsTime;
    DataItem * moveCreaturesTime;
    DataItem * prepareActionsTime;
    DataItem * executeActionsTime;
    DataItem * populationControlTime;
    DataItem * vegetalisationTime;
    DataItem * brainProcessingTime;

    DataItem * population;

    DataItem * totalEnergy;
    DataItem * availableEnergy;
    DataItem * foodEnergy;
    DataItem * creaturesEnergy;

public:
    DataAnalyser();

    DataItem *getTickTime() const;

    DataItem *getBetweenTickTime() const;

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
};


#endif //CREATURES_DATAANALYSER_H
