//
// Created by Amalric Lombard de Buffières on 8/3/20.
//

#ifndef CREATURES_LINKEVOLUTION_H
#define CREATURES_LINKEVOLUTION_H


#include "Evolution.h"

class LinkEvolution: public Evolution {
private:
    int inputGenerationNumber, outputGenerationNumber;
public:
    LinkEvolution();
    void generateFromRandom(BrainConnector * connector) override;
    void perform(BrainConnector * connector) override;
    void describe() override;

    void generateFromNeurons(BrainConnector * connector, Neuron * input, Neuron * output);

};


#endif //CREATURES_LINKEVOLUTION_H
