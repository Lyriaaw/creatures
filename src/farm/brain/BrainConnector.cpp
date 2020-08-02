//
// Created by Amalric Lombard de Buffières on 8/2/20.
//

#include "BrainConnector.h"

Creature *BrainConnector::getCreature() const {
    return creature;
}

void BrainConnector::setCreature(Creature *creature) {
    BrainConnector::creature = creature;
}

Brain *BrainConnector::getBrain() const {
    return brain;
}

void BrainConnector::setBrain(Brain *brain) {
    BrainConnector::brain = brain;
}

BrainConnector::BrainConnector(Creature *creature, Brain *brain) : creature(creature), brain(brain) {}


void BrainConnector::processBrainInputs() {
    for (int it = 0; it < brain->getInputNeurons().size(); it++) {
        brain->getInputNeurons().at(it)->fetchInputValue(creature);
    }
}