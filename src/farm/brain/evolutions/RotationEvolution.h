//
// Created by Amalric Lombard de Buffières on 8/3/20.
//

#ifndef CREATURES_ROTATIONEVOLUTION_H
#define CREATURES_ROTATIONEVOLUTION_H


#include "Evolution.h"

class RotationEvolution: public Evolution {
public:
    RotationEvolution();
    void generateFromRandom(BrainConnector * connector) override;
    void perform(BrainConnector * connector) override;
    std::string describe() override;
    Evolution * generateWithMate(Evolution * mate) override;
};


#endif //CREATURES_ROTATIONEVOLUTION_H
