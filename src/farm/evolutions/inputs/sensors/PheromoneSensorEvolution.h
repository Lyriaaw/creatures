//
// Created by Amalric Lombard de Buffières on 10/20/20.
//

#ifndef CREATURES_PHEROMONESENSOREVOLUTION_H
#define CREATURES_PHEROMONESENSOREVOLUTION_H


#include "../../Evolution.h"

class PheromoneSensorEvolution: public Evolution {
private:
    float color;
public:
    PheromoneSensorEvolution();
    void generateFromRandom(Life * life) override;
    void perform(Life * life) override;
    std::string describe() override;
    Evolution * generateWithMate(Evolution * mate) override;
    std::string getName() override;
};


#endif //CREATURES_PHEROMONESENSOREVOLUTION_H
