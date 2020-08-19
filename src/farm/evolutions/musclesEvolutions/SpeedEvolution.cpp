//
// Created by Amalric Lombard de Buffières on 8/3/20.
//

#include "SpeedEvolution.h"
#include "../../life/muscles/internals/Movement.h"

SpeedEvolution::SpeedEvolution(): Evolution() {

}

void SpeedEvolution::generateFromRandom(Life * life) {
}

void SpeedEvolution::perform(Life * life) {
    InternalMuscle * movement = new Movement(0.0, life->getEntity());

    OutputNeuron * outputNeuron = new OutputNeuron();
    outputNeuron->setName("Movement");
    outputNeuron->setGenerationNumber(this->generationNumber);
    life->getBrain()->addOutputNeuron(outputNeuron);

    movement->addNeuron(outputNeuron);

    life->addInternalMuscle(movement);
}

std::string SpeedEvolution::describe() {
    return "#" +  std::to_string(this->generationNumber) + ": Movement";
}

Evolution * SpeedEvolution::generateWithMate(Evolution * mate) {
    SpeedEvolution * childEvolution = new SpeedEvolution();
    childEvolution->setGenerationNumber(getGenerationNumber());
    return childEvolution;
}
