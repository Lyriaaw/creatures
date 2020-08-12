//
// Created by Amalric Lombard de Buffières on 8/10/20.
//

#ifndef CREATURES_SELFENERGYSENSOR_H
#define CREATURES_SELFENERGYSENSOR_H


#include "Sensor.h"

class SelfEnergySensor: public Sensor {
public:
    SelfEnergySensor(Entity *entity);

    std::string getName() override;

    void fetchSensorValue(std::vector<Entity *> accessibleEntities, std::vector<Tile *> availableTiles) override;

    void findSelectedChunks() override;
};


#endif //CREATURES_SELFENERGYSENSOR_H
