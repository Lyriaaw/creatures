//
// Created by Amalric Lombard de Buffières on 8/3/20.
//

#ifndef CREATURES_OUTPUTNEURON_H
#define CREATURES_OUTPUTNEURON_H


#include "Neuron.h"
#include "../../entities/Creature.h"

class OutputNeuron: public Neuron {
public:
    OutputNeuron();

    float getValue() override;

    virtual void transferOutputValue(Creature * creature) = 0;

};

// #################
class SpeedOutputNeuron: public OutputNeuron {
public:
    SpeedOutputNeuron();

    void transferOutputValue(Creature * creature);

};

// #################
class RotationOutputNeuron: public OutputNeuron {
public:
    RotationOutputNeuron();

    void transferOutputValue(Creature * creature);

};

// #################
class MouthOutputNeuron: public OutputNeuron {
public:
    MouthOutputNeuron();

    void transferOutputValue(Creature * creature);

};


// #################
class GenitalsOutputNeuron: public OutputNeuron {
public:
    GenitalsOutputNeuron();

    void transferOutputValue(Creature * creature);

};









#endif //CREATURES_OUTPUTNEURON_H
