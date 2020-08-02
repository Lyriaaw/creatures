//
// Created by Amalric Lombard de Buffières on 8/2/20.
//

#include <iostream>
#include "SensorEvolution.h"
#include "../neurons/SensorNeuron.h"

SensorEvolution::SensorEvolution(): Evolution() {

}

void SensorEvolution::generateFromRandom(BrainConnector * connector) {
    sensorLength = (((rand() % 300) / 100.f) + 2) * connector->getCreature()->getSize();
    sensorRotation = ((rand() % 200) / 100.f) - 1.f ;
    sensorColor = ((rand() % 999) / 1000.f);
}

void SensorEvolution::perform(BrainConnector * connector) {
    int index = connector->getCreature()->addVisionSensorSensor(sensorLength, sensorRotation, sensorColor);

    InputNeuron * distanceNeuron = new DistanceSensorNeuron(index);
    distanceNeuron->setHueOutline(sensorColor);
    distanceNeuron->setBrightnessOutline(0.5);
    distanceNeuron->setName("Sensor distance");
    connector->getBrain()->addInputNeuron(distanceNeuron);

    InputNeuron * brightnessNeuron = new BrightnessSensorNeuron(index);
    brightnessNeuron->setHueOutline(sensorColor);
    brightnessNeuron->setBrightnessOutline(0.5);
    distanceNeuron->setName("Sensor brightness");
    connector->getBrain()->addInputNeuron(brightnessNeuron);


}

void SensorEvolution::describe() {
    std::cout << this->generationNumber << " => Sensor => length: " << sensorLength << " rotation: " << sensorRotation << " color: " << sensorColor << std::endl;
}
