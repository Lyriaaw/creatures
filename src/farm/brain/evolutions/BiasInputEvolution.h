//
// Created by Amalric Lombard de Buffières on 8/3/20.
//

#ifndef CREATURES_BIASINPUTEVOLUTION_H
#define CREATURES_BIASINPUTEVOLUTION_H


#include "Evolution.h"

class BiasInputEvolution: public Evolution {
public:
    BiasInputEvolution();
    void generateFromRandom(BrainConnector * connector) override;
    void perform(BrainConnector * connector) override;
    void describe() override;
};


#endif //CREATURES_BIASINPUTEVOLUTION_H
