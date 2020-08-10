//
// Created by Amalric Lombard de Buffières on 8/2/20.
//

#include "InputNeuron.h"

InputNeuron::InputNeuron(): Neuron() {}

float InputNeuron::getValue() {
    return this->value;
}

void InputNeuron::setValue(float value) {
    this->value = value;
}
