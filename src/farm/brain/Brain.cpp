//
// Created by Amalric Lombard de Buffières on 8/2/20.
//

#include "Brain.h"

const std::vector<Neuron *> &Brain::getNeurons() const {
    return neurons;
}

void Brain::addNeuron(Neuron * neuron){
    this->neurons.push_back(neuron);
}

void Brain::addInputNeuron(InputNeuron * neuron){
    int inputNeuronCount = inputNeurons.size();

    neuron->setX(0.f);
    neuron->setY(10.f * float(inputNeuronCount));

    this->inputNeurons.push_back(neuron);
    this->neurons.push_back(neuron);
}


void Brain::setNeurons(const std::vector<Neuron *> &neurons) {
    Brain::neurons = neurons;
}

const std::vector<InputNeuron *> &Brain::getInputNeurons() const {
    return inputNeurons;
}
