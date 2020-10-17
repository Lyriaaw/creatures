//
// Created by Amalric Lombard de Buffières on 8/5/20.
//

#ifndef CREATURES_ENERGYSENSOREVOLUTION_H
#define CREATURES_ENERGYSENSOREVOLUTION_H


#include "../../Evolution.h"

class EnergySensorEvolution: public Evolution {
public:
    EnergySensorEvolution();
    void generateFromRandom(Life * life) override;
    void perform(Life * life) override;
    std::string describe() override;
    Evolution * generateWithMate(Evolution * mate) override;
    std::string getName() override;
};


#endif //CREATURES_ENERGYSENSOREVOLUTION_H
