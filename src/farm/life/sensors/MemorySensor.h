//
// Created by Amalric Lombard de Buffières on 10/16/20.
//

#ifndef CREATURES_MEMORYSENSOR_H
#define CREATURES_MEMORYSENSOR_H


#include "Sensor.h"
#include "../muscles/Muscle.h"
#include "../muscles/internals/MemoryMuscle.h"

class MemorySensor: public Sensor {
private:
    MemoryMuscle * memoryMuscle;
public:
    MemorySensor(Entity *entity, MemoryMuscle * memoryMuscle);

    std::string getName() override;

    void fetchSensorValue(std::vector<Entity *> accessibleEntities) override;

    void findSelectedChunks() override;
};


#endif //CREATURES_MEMORYSENSOR_H
