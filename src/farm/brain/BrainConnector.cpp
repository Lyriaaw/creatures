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
    for (int it = 0; it < brain->getNeurons().size(); it++) {
        brain->getNeurons().at(it)->clear();
    }

    for (int it = 0; it < brain->getInputNeurons().size(); it++) {
        brain->getInputNeurons().at(it)->fetchInputValue(creature);
    }
}



void BrainConnector::processBrain() {

    for (int it = 0; it < brain->getLinksGrid().size(); it++) {
        std::vector<Link *> linksLine = brain->getLinksGrid().at(it);

        for (int jt = 0; jt < linksLine.size(); jt++) {
            Link * link = linksLine.at(jt);

            float inputValue = link->getInput()->getValue();
            float finalValue = inputValue * link->getWeight();
            link->getOutput()->addValue(finalValue);
        }
    }
}


void BrainConnector::processBrainOutputs() {

    for (int it = 0; it < brain->getOutputNeurons().size(); it++) {
        brain->getOutputNeurons().at(it)->transferOutputValue(creature);
    }
}

