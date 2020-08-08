//
// Created by Amalric Lombard de Buffières on 8/2/20.
//

#ifndef CREATURES_BRAINCONNECTOR_H
#define CREATURES_BRAINCONNECTOR_H


#include "../entities/Creature.h"
#include "Brain.h"

class BrainConnector {
private:
    Creature * creature;
    Brain * brain;

public:
    BrainConnector(Creature *creature, Brain *brain);

    Creature *getCreature() const;

    void setCreature(Creature *creature);

    Brain *getBrain() const;

    void setBrain(Brain *brain);

    void processBrainInputs();
    void processBrain();
    void processBrainOutputs();


};


#endif //CREATURES_BRAINCONNECTOR_H
