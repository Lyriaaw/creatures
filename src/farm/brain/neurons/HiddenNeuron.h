//
// Created by Amalric Lombard de Buffières on 10/18/20.
//

#ifndef CREATURES_HIDDENNEURON_H
#define CREATURES_HIDDENNEURON_H


#include "Neuron.h"

class HiddenNeuron: public Neuron {
public:
    HiddenNeuron();

    float getValue() override ;

    void setValue(float value);
};


#endif //CREATURES_HIDDENNEURON_H
