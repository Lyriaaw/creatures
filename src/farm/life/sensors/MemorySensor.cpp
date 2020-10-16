//
// Created by Amalric Lombard de Buffières on 10/16/20.
//

#include "MemorySensor.h"
#include "../muscles/Muscle.h"
#include "../muscles/internals/MemoryMuscle.h"

std::string MemorySensor::getName() {
    return "Energy Sensor";
}

void MemorySensor::fetchSensorValue(std::vector<Entity *> accessibleEntities) {
    value = memoryMuscle->getValue();
}

void MemorySensor::findSelectedChunks() {
}

MemorySensor::MemorySensor(Entity *entity, MemoryMuscle * memoryMuscle) : Sensor(entity), memoryMuscle(memoryMuscle) {}
