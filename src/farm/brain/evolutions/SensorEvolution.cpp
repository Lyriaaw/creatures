//
// Created by Amalric Lombard de Buffières on 8/2/20.
//

#include <iostream>
#include "SensorEvolution.h"
#include "../neurons/SensorNeuron.h"

SensorEvolution::SensorEvolution(): Evolution() {

}

void SensorEvolution::generateFromRandom(Creature * creature) {
    sensorLength = (((rand() % 300) / 100.f) + 2) * creature->getSize();
    sensorRotation = ((rand() % 200) / 100.f) - 1.f ;
    sensorColor = ((rand() % 999) / 1000.f);
}

void SensorEvolution::perform(Creature * creature) {
    int index = creature->addVisionSensorSensor(sensorLength, sensorRotation, sensorColor);
    Neuron * neuron = new DistanceSensorNeuron(index);

    creature->getBrain()->addNeuron(neuron);
}

void SensorEvolution::describe() {
    std::cout << this->generationNumber << " => Sensor => length: " << sensorLength << " rotation: " << sensorRotation << " color: " << sensorColor << std::endl;
}
