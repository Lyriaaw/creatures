//
// Created by Amalric Lombard de Buffières on 8/10/20.
//

#ifndef CREATURES_BRIGHTNESSBARSENSOR_H
#define CREATURES_BRIGHTNESSBARSENSOR_H


#include "BarSensor.h"

class BrightnessBarSensor: public BarSensor {
private:
    float color;
public:
    BrightnessBarSensor(Entity *entity, float rotation, float length, float color);

    void processSensorValue(float distance, Entity *closestEntity) override;

    float getColor() const;

    void setColor(float color);

    std::string getName() override;
};


#endif //CREATURES_BRIGHTNESSBARSENSOR_H
