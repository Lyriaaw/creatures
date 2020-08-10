//
// Created by Amalric Lombard de Buffières on 8/10/20.
//

#ifndef CREATURES_SIZEBARSENSOR_H
#define CREATURES_SIZEBARSENSOR_H


#include "BarSensor.h"

class SizeBarSensor: public BarSensor {
public:
    SizeBarSensor(Entity *entity, float rotation, float length);

    void processSensorValue(float distance, Entity *closestEntity) override;

    std::string getName() override;
};


#endif //CREATURES_SIZEBARSENSOR_H
