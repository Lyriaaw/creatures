//
// Created by Amalric Lombard de Buffières on 8/2/20.
//

#ifndef CREATURES_BARSENSORDISTANCEEVOLUTION_H
#define CREATURES_BARSENSORDISTANCEEVOLUTION_H


#include "../Evolution.h"
#include "../../brain/BrainConnector.h"

class BarSensorDistanceEvolution: public Evolution {
public:
    BarSensorDistanceEvolution();
    void generateFromRandom(Life * life) override;
    void perform(Life * life) override;
    std::string describe() override;
    Evolution * generateWithMate(Evolution * mate) override;
    Evolution * generateFromCastedMate(BarSensorDistanceEvolution * mate);
private:
    float sensorLength;
    float sensorRotation;
    float sensorColor;

};


#endif //CREATURES_BARSENSORDISTANCEEVOLUTION_H
