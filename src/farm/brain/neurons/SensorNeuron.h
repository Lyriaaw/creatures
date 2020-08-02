//
// Created by Amalric Lombard de Buffières on 8/2/20.
//

#ifndef CREATURES_SENSORNEURON_H
#define CREATURES_SENSORNEURON_H


#include "InputNeuron.h"
#include "../../Creature.h"

class SensorNeuron: public InputNeuron {
protected:
    int sensorIndex;
public:
    SensorNeuron(int index);
    virtual void fetchInputValue(Creature * creature) = 0;
};




// ##################

class DistanceSensorNeuron: public SensorNeuron {
public:
    DistanceSensorNeuron(int index);
    void fetchInputValue(Creature * creature);
};

// ##################

class BrightnessSensorNeuron: public SensorNeuron {
public:
    BrightnessSensorNeuron(int index);
    void fetchInputValue(Creature * creature);
};





#endif //CREATURES_SENSORNEURON_H
