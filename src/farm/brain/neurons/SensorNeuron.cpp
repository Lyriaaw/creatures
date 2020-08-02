//
// Created by Amalric Lombard de Buffières on 8/2/20.
//

#include "SensorNeuron.h"

SensorNeuron::SensorNeuron(int index): sensorIndex(index) {

}

DistanceSensorNeuron::DistanceSensorNeuron(int index): SensorNeuron(index) {

}

void DistanceSensorNeuron::fetchInputValue(Creature * creature) {
    this->value = creature->getSensorDistance(this->sensorIndex);
}