//
// Created by Amalric Lombard de Buffières on 8/2/20.
//

#ifndef CREATURES_CREATURENURSERY_H
#define CREATURES_CREATURENURSERY_H


#include "../Creature.h"
#include "evolutions/Evolution.h"
#include "evolutions/SensorEvolution.h"
#include "evolutions/EvolutionLibrary.h"

class CreatureNursery {
private:
    EvolutionLibrary evolutionLibrary;
public:
    CreatureNursery();

    Creature * generateFromRandom();

    const EvolutionLibrary &getEvolutionLibrary() const;
};


#endif //CREATURES_CREATURENURSERY_H
