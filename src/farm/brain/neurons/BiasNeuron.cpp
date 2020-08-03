//
// Created by Amalric Lombard de Buffières on 8/3/20.
//

#include "BiasNeuron.h"

BiasNeuron::BiasNeuron(): InputNeuron() {

}

void BiasNeuron::fetchInputValue(Creature * creature) {
    this->value = 1;
}