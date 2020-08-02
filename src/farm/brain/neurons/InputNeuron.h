//
// Created by Amalric Lombard de Buffières on 8/2/20.
//

#ifndef CREATURES_INPUTNEURON_H
#define CREATURES_INPUTNEURON_H


#include "Neuron.h"
#include "../../Creature.h"

class InputNeuron: public Neuron {
public:
    virtual void fetchInputValue(Creature * creature) = 0;
};


#endif //CREATURES_INPUTNEURON_H
