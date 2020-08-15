//
// Created by Amalric Lombard de Buffières on 8/10/20.
//

#include "SelfEnergySensor.h"

std::string SelfEnergySensor::getName() {
    return "Energy Sensor";
}

void SelfEnergySensor::fetchSensorValue(std::vector<Entity *> accessibleEntities, std::vector<Tile *> availableTiles) {
    this->value = this->energyManagement->getEnergy() / this->entity->getMass();
}

void SelfEnergySensor::findSelectedTiles() {
}

SelfEnergySensor::SelfEnergySensor(Entity *entity, EnergyManagement * energyManagement) : Sensor(entity), energyManagement(energyManagement) {}
