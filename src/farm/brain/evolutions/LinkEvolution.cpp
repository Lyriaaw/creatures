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
    LinkEvolution * mateSensorEvolution(nullptr);

    if (mate == nullptr) {
        std::cout << "NULL POINTER OF MATE LINK: " << generationNumber << std::endl;
    } else {
        mateSensorEvolution = dynamic_cast<LinkEvolution *>(mate);
        if (mateSensorEvolution == nullptr) {
            std::cout << "UNABLE TO CAST FATHER COLOR EVOLUTION" << std::endl;
        }
    }
    return generateFromCastedMate(mateSensorEvolution);
}

Evolution * LinkEvolution::generateFromCastedMate(LinkEvolution * mate) {
    LinkEvolution * childSensorEvolution = new LinkEvolution();
    childSensorEvolution->setGenerationNumber(getGenerationNumber());

    childSensorEvolution->inputGenerationNumber = inputGenerationNumber;
    childSensorEvolution->outputGenerationNumber = outputGenerationNumber;

    if (mate == nullptr) {
        childSensorEvolution->weight = this->weight;
        return childSensorEvolution;
    }


    // 3 chances out of 4 to simply select one of the parent's sensor
    if (rand() % 4 != 0) {
        if (rand() % 2 == 0) {
            childSensorEvolution->weight = this->weight;
        } else {
            childSensorEvolution->weight = mate->weight;
        }

        return childSensorEvolution;
    }

    float newWeight = (this->weight + mate->weight) / 2.f;

    childSensorEvolution->weight = newWeight;

    return childSensorEvolution;
}