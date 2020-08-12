//
// Created by Amalric Lombard de Buffières on 8/12/20.
//

#ifndef CREATURES_ACCESSIBLEGROUNDENERGY_H
#define CREATURES_ACCESSIBLEGROUNDENERGY_H


#include "Sensor.h"

class AccessibleGroundEnergy: public Sensor{
public:
    std::string getName() override;

    void fetchSensorValue(std::vector<Entity *> accessibleEntities, std::vector<Tile *> availableTiles) override;

    void findSelectedChunks() override;
};


#endif //CREATURES_ACCESSIBLEGROUNDENERGY_H
