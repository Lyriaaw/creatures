//
// Created by Amalric Lombard de Buffières on 8/3/20.
//

#include "OutputNeuron.h"

OutputNeuron::OutputNeuron(): Neuron() {

}

float OutputNeuron::getValue() {
    return tanh(this->value);
}


// #################

SpeedOutputNeuron::SpeedOutputNeuron(): OutputNeuron() {

}

void SpeedOutputNeuron::transferOutputValue(Creature *creature) {
    creature->setSpeed(this->getValue());
}


// #################

RotationOutputNeuron::RotationOutputNeuron(): OutputNeuron() {

}

void RotationOutputNeuron::transferOutputValue(Creature *creature) {
    creature->setRotation(this->getValue());
}



