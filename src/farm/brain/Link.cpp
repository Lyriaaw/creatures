//
// Created by Amalric Lombard de Buffières on 8/3/20.
//

#include "Link.h"

Link::Link(float weight, Neuron *input, Neuron *output) : weight(weight), input(input), output(output) {}

float Link::getWeight() const {
    return weight;
}

void Link::setWeight(float weight) {
    Link::weight = weight;
}

Neuron *Link::getInput() const {
    return input;
}

void Link::setInput(Neuron *input) {
    Link::input = input;
}

Neuron *Link::getOutput() const {
    return output;
}

void Link::setOutput(Neuron *output) {
    Link::output = output;
}
