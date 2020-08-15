//
// Created by Amalric Lombard de Buffières on 8/10/20.
//

#ifndef CREATURES_SELFENERGYSENSOR_H
#define CREATURES_SELFENERGYSENSOR_H


#include "Sensor.h"
#include "../EnergyManagement.h"

class SelfEnergySensor: public Sensor {
private:
    EnergyManagement * energyManagement;
public:
    SelfEnergySensor(Entity *entity, EnergyManagement * energyManagement);

    std::string getName() override;

    void fetchSensorValue(std::vector<Entity *> accessibleEntities, std::vector<Tile *> availableTiles) override;

    void findSelectedTiles() override;
};


#endif //CREATURES_SELFENERGYSENSOR_H
