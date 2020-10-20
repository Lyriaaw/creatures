//
// Created by Amalric Lombard de Buffières on 8/10/20.
//

#ifndef CREATURES_SELFENERGYSENSOR_H
#define CREATURES_SELFENERGYSENSOR_H


#include "Sensor.h"
#include "../EnergyCenter.h"

class SelfEnergySensor: public Sensor {
private:
    EnergyCenter * energyCenter;
public:
    SelfEnergySensor(Entity *entity, EnergyCenter *energyCenter);

    std::string getName() override;

    void fetchSensorValue(std::vector<Entity *> accessibleEntities, std::vector<Tile *> accessibleTiles) override;

    void findSelectedChunks() override;
};


#endif //CREATURES_SELFENERGYSENSOR_H
