//
// Created by Amalric Lombard de Buffières on 8/10/20.
//

#ifndef CREATURES_DISTANCEBARSENSOR_H
#define CREATURES_DISTANCEBARSENSOR_H


#include "BarSensor.h"

class DistanceBarSensor: public BarSensor {
public:
    DistanceBarSensor(Entity *entity, float rotation, float length);

    void processSensorValue(double distance, Entity *closestEntity) override;

    std::string getName() override;
};


#endif //CREATURES_DISTANCEBARSENSOR_H
