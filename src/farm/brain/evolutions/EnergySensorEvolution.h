//
// Created by Amalric Lombard de Buffières on 8/5/20.
//

#ifndef CREATURES_ENERGYSENSOREVOLUTION_H
#define CREATURES_ENERGYSENSOREVOLUTION_H


#include "Evolution.h"

class EnergySensorEvolution: public Evolution {
private:
    float rotation;
public:
    EnergySensorEvolution();
    void generateFromRandom(BrainConnector * connector) override;
    void perform(BrainConnector * connector) override;
    std::string describe() override;
    Evolution * generateWithMate(Evolution * mate) override;
};


#endif //CREATURES_ENERGYSENSOREVOLUTION_H
