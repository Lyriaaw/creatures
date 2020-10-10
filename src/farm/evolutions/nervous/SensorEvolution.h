//
// Created by Amalric Lombard de Buffières on 8/2/20.
//

#ifndef CREATURES_SENSOREVOLUTION_H
#define CREATURES_SENSOREVOLUTION_H


#include "../Evolution.h"
#include "../../brain/BrainConnector.h"

class SensorEvolution: public Evolution {
public:
    SensorEvolution();
    void generateFromRandom(Life * life) override;
    void perform(Life * life) override;
    std::string describe() override;
    Evolution * generateWithMate(Evolution * mate) override;
    Evolution * generateFromCastedMate(SensorEvolution * mate);
private:
    float sensorLength;
    float sensorRotation;
    float sensorColor;

};


#endif //CREATURES_SENSOREVOLUTION_H