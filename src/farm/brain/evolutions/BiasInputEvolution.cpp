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

void BiasInputEvolution::describe() {
    std::cout << this->generationNumber << " => Bias Input" << std::endl;
}