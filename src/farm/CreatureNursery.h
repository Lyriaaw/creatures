//
// Created by Amalric Lombard de Buffières on 8/2/20.
//

#ifndef CREATURES_CREATURENURSERY_H
#define CREATURES_CREATURENURSERY_H


#include "entities/Creature.h"
#include "evolutions/nervous/BrightnessBarSensorEvolution.h"
#include "evolutions/inputs/BiasInputEvolution.h"
#include "evolutions/musclesEvolutions/RotationEvolution.h"
#include "evolutions/nervous/LinkEvolution.h"
#include "evolutions/musclesEvolutions/SpeedEvolution.h"
#include "evolutions/EvolutionLibrary.h"
#include "evolutions/entity/ColorEvolution.h"
#include "evolutions/entity/SizeEvolution.h"
#include "evolutions/musclesEvolutions/MouthEvolution.h"
#include "evolutions/musclesEvolutions/GenitalsEvolution.h"
#include "evolutions/inputs/sensors/EnergySensorEvolution.h"
#include "life/Life.h"

class CreatureNursery {
private:
    EvolutionLibrary evolutionLibrary;
public:
    CreatureNursery();

    Life * generateCreatureFromRandom();
    Life * Mate(Life * father, Life * mother);

    const EvolutionLibrary &getEvolutionLibrary() const;
};


#endif //CREATURES_CREATURENURSERY_H
