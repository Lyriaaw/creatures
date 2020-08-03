//
// Created by Amalric Lombard de Buffières on 8/3/20.
//

#include "LinkEvolution.h"
LinkEvolution::LinkEvolution(): Evolution() {

}

void LinkEvolution::generateFromRandom(BrainConnector * connector) {
}

void LinkEvolution::generateFromNeurons(BrainConnector * connector, Neuron * input, Neuron * output) {
    inputGenerationNumber = input->getGenerationNumber();
    outputGenerationNumber = output->getGenerationNumber();
    weight = ((rand() % 200) / 100.f) - 1.f;
}



void LinkEvolution::perform(BrainConnector * connector) {

    Neuron * inputNeuron = nullptr;
    Neuron * outputNeuron = nullptr;

    std::vector<Neuron *> connectorNeurons = connector->getBrain()->getNeurons();

    for (int it = 0; it < connectorNeurons.size(); it++) {
        Neuron * currentNeuron = connectorNeurons.at(it);

        if (currentNeuron->getGenerationNumber() == inputGenerationNumber) {
            inputNeuron = currentNeuron;
        }

        if (currentNeuron->getGenerationNumber() == outputGenerationNumber) {
            outputNeuron = currentNeuron;
        }
    }

    if (inputNeuron == nullptr || outputNeuron == nullptr) {
        std::cout << "ERROR WHILE PERFORMING LINK EVOLUTION" << std::endl;
        return;
    }

    Link * newLink = new Link(weight, inputNeuron, outputNeuron);
    connector->getBrain()->addLink(newLink);
}

std::string LinkEvolution::describe() {
    return std::to_string(this->generationNumber) + " => Link: " + std::to_string(inputGenerationNumber) + "->" + std::to_string(outputGenerationNumber);
}

Evolution * LinkEvolution::generateWithMate(Evolution * mate) {
    return nullptr;
}
