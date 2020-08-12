//
// Created by Amalric Lombard de Buffières on 8/10/20.
//

#include "SizeBarSensor.h"

void SizeBarSensor::processSensorValue(double distance, Entity *closestEntity) {
    float biggestEntity = std::max(this->entity->getSize(), closestEntity->getSize());
    float smallestEntity = std::min(this->entity->getSize(), closestEntity->getSize());

    float sizeRatio = 1 - (smallestEntity / biggestEntity);
    if (this->entity->getSize() > closestEntity->getSize()) {
        sizeRatio *= -1;
    }
    value = (0.5f + (sizeRatio / 2.f));

    std::cout << "Sie sensor" << std::endl;
}

std::string SizeBarSensor::getName() {
    return "SIZE_BAR";
}

SizeBarSensor::SizeBarSensor(Entity *entity, float rotation, float length) : BarSensor(entity, rotation, length) {}

