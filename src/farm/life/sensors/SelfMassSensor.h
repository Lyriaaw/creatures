//
// Created by Amalric Lombard de Buffières on 10/16/20.
//

#ifndef CREATURES_SELFMASSSENSOR_H
#define CREATURES_SELFMASSSENSOR_H


#include "Sensor.h"
#include "../EnergyCenter.h"

class SelfMassSensor: public Sensor {
private:
    EnergyCenter * energyCenter;
public:
    SelfMassSensor(Entity *entity, EnergyCenter *energyCenter);

    std::string getName() override;

    void fetchSensorValue(std::vector<Entity *> accessibleEntities, std::vector<Tile *> accessibleTiles) override;

    void findSelectedChunks() override;
};


#endif //CREATURES_SELFMASSSENSOR_H
