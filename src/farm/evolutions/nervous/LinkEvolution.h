//
// Created by Amalric Lombard de Buffières on 8/3/20.
//

#ifndef CREATURES_LINKEVOLUTION_H
#define CREATURES_LINKEVOLUTION_H


#include "../Evolution.h"

class LinkEvolution: public Evolution {
private:
    int inputGenerationNumber, outputGenerationNumber;
    float weight;
public:
    LinkEvolution();
    void generateFromRandom(Life * life) override;
    void perform(Life * life) override;
    std::string describe() override;
    Evolution * generateWithMate(Evolution * mate) override;
    Evolution * generateFromCastedMate(LinkEvolution * mate);

    void generateFromNeurons(Life * life, Neuron * input, Neuron * output);

};


#endif //CREATURES_LINKEVOLUTION_H
