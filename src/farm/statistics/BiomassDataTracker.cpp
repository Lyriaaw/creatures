//
// Created by Amalric Lombard de Buffières on 10/26/20.
//

#include <nlohmann/json.hpp>
#include "BiomassDataTracker.h"

BiomassDataTracker::BiomassDataTracker() {
    totalEnergy = new DataItem("totalEnergy", true);
    entitiesMass = new DataItem("entitiesMass", true);
    creaturesAvailableEnergy = new DataItem("creaturesAvailableEnergy", true);
    creaturesMass = new DataItem("creaturesMass", true);
    creaturesWastedEnergy = new DataItem("creaturesWastedEnergy", true);
    heatEnergy = new DataItem("heatEnergy", true);
    groundEnergy = new DataItem("groundEnergy", true);
    addedHeatEnergy = new DataItem("addedHeatEnergy", true);
    addedGroundEnergy = new DataItem("addedGroundEnergy", true);
}

nlohmann::json BiomassDataTracker::getLastTickAsJson() {
    nlohmann::json dto = {{ "report", {
        {"totalEnergy", totalEnergy->getLastValue()},
        {"entitiesMass", entitiesMass->getLastValue()},
        {"creaturesAvailableEnergy", creaturesAvailableEnergy->getLastValue()},
        {"creaturesMass", creaturesMass->getLastValue()},
        {"creaturesWastedEnergy", creaturesWastedEnergy->getLastValue()},
        {"heatEnergy", heatEnergy->getLastValue()},
        {"groundEnergy", groundEnergy->getLastValue()},
        {"addedHeatEnergy", addedHeatEnergy->getLastValue()},
        {"addedGroundEnergy", addedGroundEnergy->getLastValue()},
    }}};

    return dto;
}


DataItem *BiomassDataTracker::getTotalEnergy() const {
    return totalEnergy;
}

DataItem *BiomassDataTracker::getEntitiesMass() const {
    return entitiesMass;
}

DataItem *BiomassDataTracker::getCreaturesAvailableEnergy() const {
    return creaturesAvailableEnergy;
}

DataItem *BiomassDataTracker::getCreaturesMass() const {
    return creaturesMass;
}

DataItem *BiomassDataTracker::getCreaturesWastedEnergy() const {
    return creaturesWastedEnergy;
}

DataItem *BiomassDataTracker::getHeatEnergy() const {
    return heatEnergy;
}

DataItem *BiomassDataTracker::getGroundEnergy() const {
    return groundEnergy;
}

DataItem *BiomassDataTracker::getAddedHeatEnergy() const {
    return addedHeatEnergy;
}

DataItem *BiomassDataTracker::getAddedGroundEnergy() const {
    return addedGroundEnergy;
}
