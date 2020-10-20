//
// Created by Amalric Lombard de Buffières on 8/10/20.
//

#include "SelfEnergySensor.h"

std::string SelfEnergySensor::getName() {
    return "Energy Sensor";
}

void SelfEnergySensor::fetchSensorValue(std::vector<Entity *> accessibleEntities, std::vector<Tile *> accessibleTiles) {
    this->value = this->energyCenter->getAvailableEnergy() / this->energyCenter->getMaxMass();
}

void SelfEnergySensor::findSelectedChunks() {
}

SelfEnergySensor::SelfEnergySensor(Entity *entity, EnergyCenter *energyCenter) : Sensor(entity), energyCenter(energyCenter) {}
