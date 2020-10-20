//
// Created by Amalric Lombard de Buffières on 8/10/20.
//

#ifndef CREATURES_BIASSENSOR_H
#define CREATURES_BIASSENSOR_H


#include "Sensor.h"

class BiasSensor: public Sensor {
public:
    BiasSensor(Entity *entity);

    std::string getName() override;

    void fetchSensorValue(std::vector<Entity *> accessibleEntities, std::vector<Tile *> accessibleTiles) override;

    void findSelectedChunks() override;
};


#endif //CREATURES_BIASSENSOR_H
