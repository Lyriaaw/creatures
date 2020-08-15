//
// Created by Amalric Lombard de Buffières on 8/12/20.
//

#ifndef CREATURES_TILEHEATSENSOR_H
#define CREATURES_TILEHEATSENSOR_H


#include "Sensor.h"

class TileHeatSensor: public Sensor {
public:
    TileHeatSensor(Entity *entity);

    std::string getName() override;

    void fetchSensorValue(std::vector<Entity *> accessibleEntities, std::vector<Tile *> availableTiles) override;

    void findSelectedTiles() override;
};


#endif //CREATURES_TILEHEATSENSOR_H
