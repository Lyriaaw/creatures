//
// Created by Amalric Lombard de Buffières on 8/3/20.
//

#include "OutputNeuron.h"

OutputNeuron::OutputNeuron(): Neuron() {

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



