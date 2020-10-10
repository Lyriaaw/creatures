//
// Created by Amalric Lombard de Buffières on 8/5/20.
//

#include "EnergySensorNeuron.h"

EnergySensorNeuron::EnergySensorNeuron(): InputNeuron() {

}

void EnergySensorNeuron::fetchInputValue(Creature * creature) {
    this->value = creature->getEnergy() / creature->getMaxEnergy();
}