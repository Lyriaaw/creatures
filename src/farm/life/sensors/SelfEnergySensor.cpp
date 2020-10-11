//
// Created by Amalric Lombard de Buffières on 8/10/20.
//

#include "SelfEnergySensor.h"

std::string SelfEnergySensor::getName() {
    return "Energy Sensor";
}

void SelfEnergySensor::fetchSensorValue(std::vector<Entity *> accessibleEntities) {
    this->value = this->entity->getMass() / this->entity->getMaxMass();
}

void SelfEnergySensor::findSelectedChunks() {
}

SelfEnergySensor::SelfEnergySensor(Entity *entity) : Sensor(entity) {}
