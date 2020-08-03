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

void RotationEvolution::describe() {
    std::cout << this->generationNumber << " => Rotation output " << std::endl;
}