//
// Created by Amalric Lombard de Buffières on 8/3/20.
//

#include "OutputNeuron.h"

OutputNeuron::OutputNeuron(): Neuron() {

}

float OutputNeuron::getValue() {
    if (isnan(this->value)) {
        std::cout << "NaN value at neuron output" << std::endl;
        return 0.0;
    }
    return tanh(this->value);
}


// #################

SpeedOutputNeuron::SpeedOutputNeuron(): OutputNeuron() {

}

void SpeedOutputNeuron::transferOutputValue(Creature *creature) {
//    creature->setSpeed(this->getValue());
}


// #################

RotationOutputNeuron::RotationOutputNeuron(): OutputNeuron() {

}

void RotationOutputNeuron::transferOutputValue(Creature *creature) {
    creature->setRotation(this->getValue());
}



// #################

MouthOutputNeuron::MouthOutputNeuron(): OutputNeuron() {

}

void MouthOutputNeuron::transferOutputValue(Creature *creature) {
    creature->setMouthValue((this->getValue() / 2.f) + 0.5f);
}



// #################
GenitalsOutputNeuron::GenitalsOutputNeuron(): OutputNeuron() {

}

void GenitalsOutputNeuron::transferOutputValue(Creature *creature) {
    creature->setGenitalsValue(this->getValue());
}



