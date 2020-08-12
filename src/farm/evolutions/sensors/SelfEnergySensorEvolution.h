//
// Created by Amalric Lombard de Buffières on 8/5/20.
//

#ifndef CREATURES_SELFENERGYSENSOREVOLUTION_H
#define CREATURES_SELFENERGYSENSOREVOLUTION_H


#include "../Evolution.h"

class SelfEnergySensorEvolution: public Evolution {
private:
    float rotation;
public:
    SelfEnergySensorEvolution();
    void generateFromRandom(Life * life) override;
    void perform(Life * life) override;
    std::string describe() override;
    Evolution * generateWithMate(Evolution * mate) override;
};


#endif //CREATURES_SELFENERGYSENSOREVOLUTION_H
