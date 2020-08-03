//
// Created by Amalric Lombard de Buffières on 8/3/20.
//

#ifndef CREATURES_SPEEDEVOLUTION_H
#define CREATURES_SPEEDEVOLUTION_H


#include "Evolution.h"

class SpeedEvolution: public Evolution {
public:
    SpeedEvolution();
    void generateFromRandom(BrainConnector * connector) override;
    void perform(BrainConnector * connector) override;
    void describe() override;
};


#endif //CREATURES_SPEEDEVOLUTION_H
