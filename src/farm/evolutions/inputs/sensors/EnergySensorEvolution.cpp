//
// Created by Amalric Lombard de Buffières on 8/5/20.
//

#include "EnergySensorEvolution.h"
#include "../../../brain/neurons/EnergySensorNeuron.h"

EnergySensorEvolution::EnergySensorEvolution(): Evolution() {

}

void EnergySensorEvolution::generateFromRandom(BrainConnector * connector) {
}

void EnergySensorEvolution::perform(BrainConnector * connector) {
    InputNeuron * bias = new EnergySensorNeuron();
    bias->setName("Energy");
    bias->setGenerationNumber(this->generationNumber);
    connector->getBrain()->addInputNeuron(bias);
}

std::string EnergySensorEvolution::describe() {
    return std::to_string(this->generationNumber) + " => Energy sensor";
}

Evolution * EnergySensorEvolution::generateWithMate(Evolution * mate) {
    EnergySensorEvolution * childEvolution = new EnergySensorEvolution();
    childEvolution->setGenerationNumber(getGenerationNumber());
    return childEvolution;
}
