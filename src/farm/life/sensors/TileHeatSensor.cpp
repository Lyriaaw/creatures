//
// Created by Amalric Lombard de Buffières on 8/12/20.
//

#include "TileHeatSensor.h"
std::string TileHeatSensor::getName() {
    return "TILE_HEAT_SENSOR";
}

void TileHeatSensor::fetchSensorValue(std::vector<Entity *> accessibleEntities, std::vector<Tile *> availableTiles) {

    double tileValue = 0.0;
    for (int it = 0; it < selectedChunks.size(); it++) {

        for (int jt = 0; jt < availableTiles.size(); jt++) {
            if (!selectedChunks.at(it).equals(availableTiles.at(jt)->getPosition())) {
                continue;
            }

            tileValue = availableTiles.at(jt)->getHeat();
        }
    }


    float biggestEntity = std::max(this->entity->getMaxEnergy(), tileValue);
    float smallestEntity = std::min(this->entity->getMaxEnergy(), tileValue);

    float sizeRatio = 1 - (smallestEntity / biggestEntity);
    if (this->entity->getMaxEnergy() > tileValue) {
        sizeRatio *= -1;
    }
    value = (0.5f + (sizeRatio / 2.f));
}

void TileHeatSensor::findSelectedChunks() {
    this->selectedChunks.clear();

    Point entityPoint = this->entity->getPosition();
    Point tilePoint = entityPoint.getTileCoordinates();


    selectedChunks.emplace_back(tilePoint);
}

TileHeatSensor::TileHeatSensor(Entity *entity) : Sensor(entity) {}
