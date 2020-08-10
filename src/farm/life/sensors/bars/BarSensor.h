//
// Created by Amalric Lombard de Buffières on 8/9/20.
//

#ifndef CREATURES_BARSENSOR_H
#define CREATURES_BARSENSOR_H


#include "../Sensor.h"

class BarSensor: public Sensor {
protected:
    float rotation;
    float length;

public:
    BarSensor(Entity *entity, float rotation, float length);

    void findSelectedChunks() override;
    void fetchSensorValue(std::vector<Entity *> accessibleEntities) override;
    void getSensorValueFromSensorEquation(float sensorX, float sensorY, float m, float p, std::vector<Entity *> accessibleEntities);

    virtual void processSensorValue(float distance, Entity * closestEntity) = 0;

};


#endif //CREATURES_BARSENSOR_H
