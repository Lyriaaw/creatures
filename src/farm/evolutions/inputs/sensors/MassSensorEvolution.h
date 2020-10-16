//
// Created by Amalric Lombard de Buffières on 10/16/20.
//

#ifndef CREATURES_MASSSENSOREVOLUTION_H
#define CREATURES_MASSSENSOREVOLUTION_H


#include "../../Evolution.h"

class MassSensorEvolution: public Evolution {
public:
    MassSensorEvolution();
    void generateFromRandom(Life * life) override;
    void perform(Life * life) override;
    std::string describe() override;
    Evolution * generateWithMate(Evolution * mate) override;
};


#endif //CREATURES_MASSSENSOREVOLUTION_H
