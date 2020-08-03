//
// Created by Amalric Lombard de Buffières on 8/3/20.
//

#include "SpeedEvolution.h"

SpeedEvolution::SpeedEvolution(): Evolution() {

}

void SpeedEvolution::generateFromRandom(BrainConnector * connector) {
}

void SpeedEvolution::perform(BrainConnector * connector) {
    OutputNeuron * speed = new SpeedOutputNeuron();
    speed->setName("Speed");
    speed->setGenerationNumber(this->generationNumber);
    connector->getBrain()->addOutputNeuron(speed);
}

std::string SpeedEvolution::describe() {
    return std::to_string(this->generationNumber) + " => Speed output ";
}

Evolution * SpeedEvolution::generateWithMate(Evolution * mate) {
    SpeedEvolution * childEvolution = new SpeedEvolution();
    childEvolution->setGenerationNumber(getGenerationNumber());
    return childEvolution;
}