//
// Created by Amalric Lombard de Buffières on 8/10/20.
//

#include "BiasSensor.h"

BiasSensor::BiasSensor(Entity *entity) : Sensor(entity) {}

std::string BiasSensor::getName() {
    return "Bias";
}

void BiasSensor::fetchSensorValue(std::vector<Entity *> accessibleEntities) {
    this->connectedNeuron->setValue(1.0);
}

void BiasSensor::findSelectedChunks() {
}
