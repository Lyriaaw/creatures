//
// Created by Amalric Lombard de Buffières on 8/2/20.
//

#ifndef CREATURES_SENSOREVOLUTION_H
#define CREATURES_SENSOREVOLUTION_H


#include "Evolution.h"

class SensorEvolution: public Evolution {
public:
    SensorEvolution();
    void generateFromRandom(Creature * creature) override;
    void perform(Creature * creature) override;
    void describe() override;
private:
    float sensorLength;
    float sensorRotation;
    float sensorColor;

};


#endif //CREATURES_SENSOREVOLUTION_H
