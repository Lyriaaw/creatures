//
// Created by Amalric Lombard de Buffières on 8/2/20.
//

#ifndef CREATURES_CREATURENURSERY_H
#define CREATURES_CREATURENURSERY_H


#include "entities/Creature.h"
#include "evolutions/nervous/HueBarSensorEvolution.h"
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
    int farmId;
    MongoClient * mongoClient;
    EvolutionLibrary * evolutionLibrary;
public:
    CreatureNursery(int farmId);

    Life * generateCreatureFromRandom();
    Life * Mate(Life * father, Life * mother, MongoClient * mongoClient, int tick);

    const EvolutionLibrary &getEvolutionLibrary() const;

    std::vector<Evolution *> generateNewRandomEvolution(Life * life, std::vector<Evolution *> childGenome);

    std::vector<Evolution *> generateNeuronEvolution(Life *life, std::vector<Evolution *> childGenome);

    std::vector<Evolution *>
    generateNeuronEvolutionFromLinkEvolution(Neuron *inputNeuron, Neuron *outputNeuron,
                                             LinkEvolution *linkEvolution, Life * life);

    std::vector<Evolution *> generateLinkEvolution(Neuron *inputNeuron, Neuron *outputNeuron, Life *life);
};


#endif //CREATURES_CREATURENURSERY_H
