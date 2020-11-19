//
// Created by Amalric Lombard de Buffières on 10/25/20.
//

#ifndef CREATURES_LIFERUNNERDATATRACKER_H
#define CREATURES_LIFERUNNERDATATRACKER_H


#include "DataItem.h"

class LifeRunnerDataTracker {
private:
    DataItem * poopCount;
    DataItem * pheromoneCount;
    DataItem * eatCount;
    DataItem * eatLifeCount;
    DataItem * mateFailureCount;
    DataItem * mateSuccessCount;
    DataItem * biteCount;
    DataItem * biteLifeCount;

    DataItem * chunkSelection;
    DataItem * accessibleEntities;
    DataItem * sensorProcessing;
    DataItem * brainProcessing;
    DataItem * externalActions;
    DataItem * moveCreatures;

    DataItem * tickTime;
    DataItem * tickPerSecond;

    DataItem * lifesCount;

public:

    LifeRunnerDataTracker();

    DataItem *getPoopCount() const;

    DataItem *getPheromoneCount() const;

    DataItem *getEatCount() const;

    DataItem *getEatLifeCount() const;

    DataItem *getMateFailureCount() const;

    DataItem *getMateSuccessCount() const;

    DataItem *getBiteCount() const;

    DataItem *getBiteLifeCount() const;

    DataItem *getChunkSelection() const;

    DataItem *getSensorProcessing() const;

    DataItem *getBrainProcessing() const;

    DataItem *getExternalActions() const;

    DataItem *getTickTime() const;

    DataItem *getTickPerSecond() const;

    DataItem *getMoveCreatures() const;

    DataItem *getLifesCount() const;

    DataItem *getAccessibleEntities() const;
};


#endif //CREATURES_LIFERUNNERDATATRACKER_H
