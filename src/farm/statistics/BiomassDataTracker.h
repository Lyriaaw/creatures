//
// Created by Amalric Lombard de Buffières on 10/26/20.
//

#ifndef CREATURES_BIOMASSDATATRACKER_H
#define CREATURES_BIOMASSDATATRACKER_H


#include "DataItem.h"

class BiomassDataTracker {
private:
    DataItem * totalEnergy;

    DataItem * entitiesMass;

    DataItem * creaturesAvailableEnergy;
    DataItem * creaturesMass;
    DataItem * creaturesWastedEnergy;

    DataItem * heatEnergy;
    DataItem * groundEnergy;
    DataItem * addedHeatEnergy;
    DataItem * addedGroundEnergy;

public:

    BiomassDataTracker();

    DataItem *getTotalEnergy() const;

    DataItem *getEntitiesMass() const;

    DataItem *getCreaturesAvailableEnergy() const;

    DataItem *getCreaturesMass() const;

    DataItem *getCreaturesWastedEnergy() const;

    DataItem *getHeatEnergy() const;

    DataItem *getGroundEnergy() const;

    DataItem *getAddedHeatEnergy() const;

    DataItem *getAddedGroundEnergy() const;


    nlohmann::json getLastTickAsJson();
};


#endif //CREATURES_BIOMASSDATATRACKER_H
