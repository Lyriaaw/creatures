//
// Created by Amalric Lombard de Buffières on 8/10/20.
//

#include "BrightnessBarSensor.h"

void BrightnessBarSensor::processSensorValue(double distance, Entity *closestEntity) {
//    float hueDistances = std::min(abs(closestEntity->getColor() - color), 1.f - abs(closestEntity->getColor() - color));
////    value = float(pow(2, - (hueDistances * 5)));
//    value = 1 - hueDistances;


    this->value = 1.f - (distance / length);
}

float BrightnessBarSensor::getColor() const {
    return color;
}

void BrightnessBarSensor::setColor(float color) {
    BrightnessBarSensor::color = color;
}

std::string BrightnessBarSensor::getName() {
    return "BRIGHTNESS_BAR";
}

BrightnessBarSensor::BrightnessBarSensor(Entity *entity, float rotation, float length, float color) : BarSensor(entity,
                                                                                                                rotation,
                                                                                                                length),
                                                                                                      color(color) {}
