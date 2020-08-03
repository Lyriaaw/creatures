//
// Created by Amalric Lombard de Buffières on 8/3/20.
//

#include "RotationEvolution.h"

RotationEvolution::RotationEvolution(): Evolution() {

}

void RotationEvolution::generateFromRandom(BrainConnector * connector) {
}

void RotationEvolution::perform(BrainConnector * connector) {
    OutputNeuron * rotation = new RotationOutputNeuron();
    rotation->setName("Rotation");
    rotation->setGenerationNumber(this->generationNumber);
    connector->getBrain()->addOutputNeuron(rotation);
}

std::string RotationEvolution::describe() {
    return std::to_string(this->generationNumber) + " => Rotation output ";
}

Evolution * RotationEvolution::generateWithMate(Evolution * mate) {
    RotationEvolution * childEvolution = new RotationEvolution();
    childEvolution->setGenerationNumber(getGenerationNumber());
    return childEvolution;
}
