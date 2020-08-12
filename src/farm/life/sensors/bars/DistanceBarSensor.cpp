//
// Created by Amalric Lombard de Buffières on 8/10/20.
//

#include "DistanceBarSensor.h"

void DistanceBarSensor::processSensorValue(double distance, Entity *closestEntity) {
    this->value = 1.f - (distance / length);
}

std::string DistanceBarSensor::getName() {
    return "DISTANCE_BAR";
}

DistanceBarSensor::DistanceBarSensor(Entity *entity, float rotation, float length) : BarSensor(entity, rotation,
                                                                                               length) {}
