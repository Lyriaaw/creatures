//
// Created by Amalric Lombard de Buffières on 10/16/20.
//

#include "SelfMassSensor.h"

std::string SelfMassSensor::getName() {
    return "Mass Sensor";
}

void SelfMassSensor::fetchSensorValue(std::vector<Entity *> accessibleEntities, std::vector<Tile *> accessibleTiles) {
    this->value = this->entity->getMass() / this->energyCenter->getMaxMass();
}

void SelfMassSensor::findSelectedChunks() {
}

SelfMassSensor::SelfMassSensor(Entity *entity, EnergyCenter *energyCenter) : Sensor(entity), energyCenter(energyCenter) {}
