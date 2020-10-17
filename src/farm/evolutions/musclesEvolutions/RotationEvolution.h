//
// Created by Amalric Lombard de Buffières on 8/3/20.
//

#ifndef CREATURES_ROTATIONEVOLUTION_H
#define CREATURES_ROTATIONEVOLUTION_H


#include "../Evolution.h"

class RotationEvolution: public Evolution {
public:
    RotationEvolution();
    void generateFromRandom(Life * life) override;
    void perform(Life * life) override;
    std::string describe() override;
    Evolution * generateWithMate(Evolution * mate) override;
    std::string getName() override;
};


#endif //CREATURES_ROTATIONEVOLUTION_H
