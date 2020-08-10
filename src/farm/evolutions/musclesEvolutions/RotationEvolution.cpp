//
// Created by Amalric Lombard de Buffières on 8/3/20.
//

#include "RotationEvolution.h"
#include "../../life/muscles/internals/Rotation.h"

RotationEvolution::RotationEvolution(): Evolution() {

}

void RotationEvolution::generateFromRandom(Life * life) {
}

void RotationEvolution::perform(Life * life) {
    InternalMuscle * rotation = new Rotation(0.0, life->getEntity());

    OutputNeuron * outputNeuron = new OutputNeuron();
    outputNeuron->setName("Rotation");
    outputNeuron->setGenerationNumber(this->generationNumber);
    life->getBrain()->addOutputNeuron(outputNeuron);

    rotation->addNeuron(outputNeuron);
}

std::string RotationEvolution::describe() {
    return std::to_string(this->generationNumber) + " => Rotation output ";
}

Evolution * RotationEvolution::generateWithMate(Evolution * mate) {
    RotationEvolution * childEvolution = new RotationEvolution();
    childEvolution->setGenerationNumber(getGenerationNumber());
    return childEvolution;
}
