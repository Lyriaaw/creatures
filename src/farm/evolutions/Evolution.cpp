//
// Created by Amalric Lombard de Buffières on 8/2/20.
//

#include "Evolution.h"

int Evolution::nextEvolutionNumber = 1;

Evolution::Evolution() {

}

int Evolution::getGenerationNumber() {
    return this->generationNumber;
}

void Evolution::setGenerationNumber(int newGenerationNumber) {
    generationNumber = newGenerationNumber;

    if (Evolution::nextEvolutionNumber <= newGenerationNumber) {
        Evolution::nextEvolutionNumber = newGenerationNumber + 1;
    }
}
void Evolution::generateGenerationNumber() {
    generationNumber = Evolution::nextEvolutionNumber;
    Evolution::nextEvolutionNumber++;
}

InputNeuron *Evolution::getInputNeuron() const {
    return inputNeuron;
}

const std::vector<OutputNeuron *> &Evolution::getOutputNeurons() const {
    return outputNeurons;
}
