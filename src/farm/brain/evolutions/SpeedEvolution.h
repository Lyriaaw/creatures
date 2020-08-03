//
// Created by Amalric Lombard de Buffières on 8/3/20.
//

#ifndef CREATURES_SPEEDANDROTATIONEVOLUTION_H
#define CREATURES_SPEEDANDROTATIONEVOLUTION_H


#include "Evolution.h"

class SpeedAndRotationEvolution: public Evolution {
public:
    SpeedAndRotationEvolution();
    void generateFromRandom(BrainConnector * connector) override;
    void perform(BrainConnector * connector) override;
    void describe() override;
};


#endif //CREATURES_SPEEDANDROTATIONEVOLUTION_H
