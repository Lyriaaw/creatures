//
// Created by Amalric Lombard de Buffières on 8/5/20.
//

#include "SelfEnergySensorEvolution.h"
#include "../../life/sensors/SelfEnergySensor.h"

SelfEnergySensorEvolution::SelfEnergySensorEvolution(): Evolution() {

}

void SelfEnergySensorEvolution::generateFromRandom(Life * life) {
}

void SelfEnergySensorEvolution::perform(Life * life) {
    InputNeuron * inputNeuron = new InputNeuron();
    inputNeuron->setName("Energy");
    inputNeuron->setGenerationNumber(this->generationNumber);

    Sensor * energySensor = new SelfEnergySensor(life->getEntity());
    energySensor->setConnectedNeuron(inputNeuron);

    life->getBrain()->addInputNeuron(inputNeuron);

    life->addSensor(energySensor);
}

std::string SelfEnergySensorEvolution::describe() {
    return std::to_string(this->generationNumber) + " => Energy sensor";
}

Evolution * SelfEnergySensorEvolution::generateWithMate(Evolution * mate) {
    SelfEnergySensorEvolution * childEvolution = new SelfEnergySensorEvolution();
    childEvolution->setGenerationNumber(getGenerationNumber());
    return childEvolution;
}
