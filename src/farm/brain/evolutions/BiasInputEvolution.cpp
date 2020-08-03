//
// Created by Amalric Lombard de Buffières on 8/3/20.
//

#include "BiasInputEvolution.h"
#include "../neurons/BiasNeuron.h"

BiasInputEvolution::BiasInputEvolution(): Evolution() {

}

void BiasInputEvolution::generateFromRandom(BrainConnector * connector) {
}

void BiasInputEvolution::perform(BrainConnector * connector) {
    InputNeuron * bias = new BiasNeuron();
    bias->setName("Bias");
    bias->setGenerationNumber(this->generationNumber);
    connector->getBrain()->addInputNeuron(bias);
}

std::string BiasInputEvolution::describe() {
    return std::to_string(this->generationNumber) + " => Bias Input";
}

Evolution * BiasInputEvolution::generateWithMate(Evolution * mate) {
    BiasInputEvolution * childEvolution = new BiasInputEvolution();
    childEvolution->setGenerationNumber(getGenerationNumber());
    return childEvolution;
}
