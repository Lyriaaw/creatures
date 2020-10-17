//
// Created by Amalric Lombard de Buffières on 8/2/20.
//

#ifndef CREATURES_HUEBARSENSOREVOLUTION_H
#define CREATURES_HUEBARSENSOREVOLUTION_H


#include "../Evolution.h"
#include "../../brain/BrainConnector.h"

class HueBarSensorEvolution: public Evolution {
public:
    HueBarSensorEvolution();
    void generateFromRandom(Life * life) override;
    void perform(Life * life) override;
    std::string describe() override;
    Evolution * generateWithMate(Evolution * mate) override;
    Evolution * generateFromCastedMate(HueBarSensorEvolution * mate);
    std::string getName() override;
private:
    float sensorLength;
    float sensorRotation;
    float sensorColor;

};


#endif //CREATURES_HUEBARSENSOREVOLUTION_H
