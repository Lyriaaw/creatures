//
// Created by Amalric Lombard de Buffières on 8/12/20.
//

#include "TileHeatSensor.h"
std::string TileHeatSensor::getName() {
    return "TILE_HEAT_SENSOR";
}

void TileHeatSensor::fetchSensorValue(std::vector<Entity *> accessibleEntities, std::vector<Tile *> availableTiles) {
    value = availableTiles.at(0)->getHeat();
}

void TileHeatSensor::findSelectedChunks() {
    Point entityPoint = this->entity->getPosition();
    Point tilePoint = entityPoint.getTileCoordinates();


    selectedChunks.emplace_back(tilePoint);
}

TileHeatSensor::TileHeatSensor(Entity *entity) : Sensor(entity) {}
