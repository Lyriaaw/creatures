//
// Created by Amalric Lombard de Buffières on 8/2/20.
//

#ifndef CREATURES_CREATURENURSERY_H
#define CREATURES_CREATURENURSERY_H


#include "entities/Creature.h"
#include "evolutions/nervous/SensorEvolution.h"
#include "evolutions/inputs/BiasInputEvolution.h"
#include "evolutions/muscles/RotationEvolution.h"
#include "evolutions/nervous/LinkEvolution.h"
#include "evolutions/muscles/SpeedEvolution.h"
#include "evolutions/EvolutionLibrary.h"
#include "evolutions/entity/ColorEvolution.h"
#include "evolutions/entity/SizeEvolution.h"
#include "evolutions/muscles/MouthEvolution.h"
#include "evolutions/muscles/GenitalsEvolution.h"
#include "evolutions/inputs/sensors/EnergySensorEvolution.h"

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
