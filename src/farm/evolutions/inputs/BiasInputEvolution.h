//
// Created by Amalric Lombard de Buffières on 8/3/20.
//

#ifndef CREATURES_BIASINPUTEVOLUTION_H
#define CREATURES_BIASINPUTEVOLUTION_H


#include "../Evolution.h"

class BiasInputEvolution: public Evolution {
public:
    BiasInputEvolution();
    void generateFromRandom(BrainConnector * connector) override;
    void perform(BrainConnector * connector) override;
    std::string describe() override;
    Evolution * generateWithMate(Evolution * mate) override;
};


#endif //CREATURES_BIASINPUTEVOLUTION_H
