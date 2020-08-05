//
// Created by Amalric Lombard de Buffières on 8/2/20.
//

#ifndef CREATURES_CREATURENURSERY_H
#define CREATURES_CREATURENURSERY_H


#include "../Creature.h"
#include "evolutions/SensorEvolution.h"
#include "evolutions/BiasInputEvolution.h"
#include "evolutions/RotationEvolution.h"
#include "evolutions/LinkEvolution.h"
#include "evolutions/SpeedEvolution.h"
#include "evolutions/EvolutionLibrary.h"
#include "evolutions/ColorEvolution.h"
#include "evolutions/SizeEvolution.h"
#include "evolutions/MouthEvolution.h"
#include "evolutions/GenitalsEvolution.h"
#include "evolutions/EnergySensorEvolution.h"

class CreatureNursery {
private:
    EvolutionLibrary evolutionLibrary;
public:
    CreatureNursery();

    BrainConnector * generateFromRandom();
    BrainConnector * Mate(BrainConnector * father, BrainConnector * mother);

    const EvolutionLibrary &getEvolutionLibrary() const;
};


#endif //CREATURES_CREATURENURSERY_H
