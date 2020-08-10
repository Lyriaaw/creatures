//
// Created by Amalric Lombard de Buffières on 8/3/20.
//

#include "BiasInputEvolution.h"
#include "../../brain/neurons/BiasNeuron.h"
#include "../../life/sensors/BiasSensor.h"

BiasInputEvolution::BiasInputEvolution(): Evolution() {

}

void BiasInputEvolution::generateFromRandom(Life * life) {
}

void BiasInputEvolution::perform(Life * life) {
    InputNeuron * inputNeuron = new InputNeuron();
    inputNeuron->setName("Bias");
    inputNeuron->setGenerationNumber(this->generationNumber);

    Sensor * energySensor = new BiasSensor(life->getEntity());
    energySensor->setConnectedNeuron(inputNeuron);

    life->getBrain()->addInputNeuron(inputNeuron);
}

std::string BiasInputEvolution::describe() {
    return std::to_string(this->generationNumber) + " => Bias Input";
}

Evolution * BiasInputEvolution::generateWithMate(Evolution * mate) {
    BiasInputEvolution * childEvolution = new BiasInputEvolution();
    childEvolution->setGenerationNumber(getGenerationNumber());
    return childEvolution;
}
