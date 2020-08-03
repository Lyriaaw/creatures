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

    float weight = ((rand() % 200) / 100.f) - 1.f;
    Link * newLink = new Link(weight, input, output);
    connector->getBrain()->addLink(newLink);
}



void LinkEvolution::perform(BrainConnector * connector) {
    OutputNeuron * rotation = new RotationOutputNeuron();
    rotation->setName("Rotation");
    rotation->setGenerationNumber(this->generationNumber);
    connector->getBrain()->addOutputNeuron(rotation);
}

void LinkEvolution::describe() {
    std::cout << this->generationNumber << " => Link: " << inputGenerationNumber << "->" << outputGenerationNumber << std::endl;
}