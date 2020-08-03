//
// Created by Amalric Lombard de Buffières on 8/3/20.
//

#ifndef CREATURES_BIASNEURON_H
#define CREATURES_BIASNEURON_H


#include "InputNeuron.h"

class BiasNeuron: public InputNeuron {
public:
    BiasNeuron();
    void fetchInputValue(Creature * creature);
};


#endif //CREATURES_BIASNEURON_H
