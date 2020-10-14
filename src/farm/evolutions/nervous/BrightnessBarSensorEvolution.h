//
// Created by Amalric Lombard de Buffières on 8/2/20.
//

#ifndef CREATURES_BRIGHTNESSBARSENSOREVOLUTION_H
#define CREATURES_BRIGHTNESSBARSENSOREVOLUTION_H


#include "../Evolution.h"
#include "../../brain/BrainConnector.h"

class BrightnessBarSensorEvolution: public Evolution {
public:
    BrightnessBarSensorEvolution();
    void generateFromRandom(Life * life) override;
    void perform(Life * life) override;
    std::string describe() override;
    Evolution * generateWithMate(Evolution * mate) override;
    Evolution * generateFromCastedMate(BrightnessBarSensorEvolution * mate);
private:
    float sensorLength;
    float sensorRotation;
    float sensorColor;

};


#endif //CREATURES_BRIGHTNESSBARSENSOREVOLUTION_H
