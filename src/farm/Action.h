//
// Created by Amalric Lombard de Buffières on 8/4/20.
//

#ifndef CREATURES_ACTION_H
#define CREATURES_ACTION_H


#include "Creature.h"

class Action {
private:
    Creature * performer;
    Entity * subject;

public:

    Action(Creature *pCreature, Entity *pEntity);
};


#endif //CREATURES_ACTION_H
