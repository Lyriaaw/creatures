//
// Created by Amalric Lombard de Buffières on 8/2/20.
//

#ifndef CREATURES_INPUTNEURON_H
#define CREATURES_INPUTNEURON_H


#include "Neuron.h"
#include "../../entities/Creature.h"

class InputNeuron: public Neuron {
public:
    InputNeuron();

    float getValue() override ;

    void setValue(float value);


};


#endif //CREATURES_INPUTNEURON_H
