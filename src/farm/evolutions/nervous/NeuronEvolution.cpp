//
// Created by Amalric Lombard de Buffières on 10/18/20.
//

#include <map>
#include "NeuronEvolution.h"
#include "../../brain/neurons/HiddenNeuron.h"

NeuronEvolution::NeuronEvolution(): Evolution() {

}

void NeuronEvolution::generateFromRandom(Life * life) {
}


void NeuronEvolution::generateFromXandY(float neuronX, float neuronY) {
    this->x = neuronX;
    this->y = neuronY;
    // TODO: Check if another neuron is already there
}

void NeuronEvolution::perform(Life * life) {
    neuron = new HiddenNeuron();
    neuron->setX(x);
    neuron->setY(y);
    neuron->setGenerationNumber(this->generationNumber);
    life->getBrain()->addNeuron(neuron);
}

std::string NeuronEvolution::describe() {
    return std::to_string(this->generationNumber) + " => Neuron: " + std::to_string(x) + " - " + std::to_string(y);
}

Evolution * NeuronEvolution::generateWithMate(Evolution * mate) {
    NeuronEvolution * mateSensorEvolution(nullptr);


    if (mate == nullptr) {
//        std::cout << "NULL POINTER OF MATE LINK: " << generationNumber << std::endl;
    } else {
        mateSensorEvolution = dynamic_cast<NeuronEvolution *>(mate);
        if (mateSensorEvolution == nullptr) {
            std::cout << "UNABLE TO CAST FATHER COLOR EVOLUTION" << std::endl;
        }
    }
    return generateFromCastedMate(mateSensorEvolution);
}

Evolution * NeuronEvolution::generateFromCastedMate(NeuronEvolution * mate) {
    NeuronEvolution * childSensorEvolution = new NeuronEvolution();
    childSensorEvolution->setGenerationNumber(getGenerationNumber());

    childSensorEvolution->x = x;
    childSensorEvolution->y = y;

    return childSensorEvolution;
}


std::string NeuronEvolution::getName() {
    return "Neuron";
}

Neuron *NeuronEvolution::getNeuron() const {
    return neuron;
}

float NeuronEvolution::getX() const {
    return x;
}

float NeuronEvolution::getY() const {
    return y;
}

