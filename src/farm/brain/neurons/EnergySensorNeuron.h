//
// Created by Amalric Lombard de Buffières on 8/5/20.
//

#ifndef CREATURES_ENERGYSENSORNEURON_H
#define CREATURES_ENERGYSENSORNEURON_H


#include "InputNeuron.h"

class EnergySensorNeuron: public InputNeuron {
public:
    EnergySensorNeuron();
    void fetchInputValue(Creature * creature);
};


#endif //CREATURES_ENERGYSENSORNEURON_H
