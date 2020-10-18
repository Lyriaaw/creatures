//
// Created by Amalric Lombard de Buffières on 10/18/20.
//

#include <cmath>
#include "HiddenNeuron.h"

HiddenNeuron::HiddenNeuron(): Neuron() {}

float HiddenNeuron::getValue() {
    return tanh(this->value);
}

void HiddenNeuron::setValue(float value) {
    this->value = value;
}
