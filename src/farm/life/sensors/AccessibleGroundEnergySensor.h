//
// Created by Amalric Lombard de Buffières on 8/12/20.
//

#ifndef CREATURES_ACCESSIBLEGROUNDENERGYSENSOR_H
#define CREATURES_ACCESSIBLEGROUNDENERGYSENSOR_H


#include "Sensor.h"

class AccessibleGroundEnergySensor: public Sensor{
public:
    AccessibleGroundEnergySensor(Entity *entity);

    std::string getName() override;

    void fetchSensorValue(std::vector<Entity *> accessibleEntities, std::vector<Tile *> availableTiles) override;

    void findSelectedTiles() override;
};


#endif //CREATURES_ACCESSIBLEGROUNDENERGYSENSOR_H
