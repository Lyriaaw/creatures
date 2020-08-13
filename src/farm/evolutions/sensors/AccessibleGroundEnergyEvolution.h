//
// Created by Amalric Lombard de Buffières on 8/12/20.
//

#ifndef CREATURES_ACCESSIBLEGROUNDENERGYEVOLUTION_H
#define CREATURES_ACCESSIBLEGROUNDENERGYEVOLUTION_H


#include "../Evolution.h"

class AccessibleGroundEnergyEvolution: public Evolution {
public:
    AccessibleGroundEnergyEvolution();

    void generateFromRandom(Life *life) override;

    void perform(Life *connector) override;

    std::string describe() override;

    Evolution *generateWithMate(Evolution *mate) override;
};


#endif //CREATURES_ACCESSIBLEGROUNDENERGYEVOLUTION_H
