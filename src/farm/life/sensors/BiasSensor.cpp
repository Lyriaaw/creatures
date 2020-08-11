//
// Created by Amalric Lombard de Buffières on 8/10/20.
//

#include "BiasSensor.h"

BiasSensor::BiasSensor(Entity *entity) : Sensor(entity) {}

std::string BiasSensor::getName() {
    return "Bias";
}

void BiasSensor::fetchSensorValue(std::vector<Entity *> accessibleEntities) {
    this->value = 1;
}

void BiasSensor::findSelectedChunks() {
}
