//
// Created by Amalric Lombard de Buffières on 8/5/20.
//

#include "EnergySensorEvolution.h"
#include "../../../brain/neurons/EnergySensorNeuron.h"
#include "../../../life/sensors/SelfEnergySensor.h"

EnergySensorEvolution::EnergySensorEvolution(): Evolution() {

}

void EnergySensorEvolution::generateFromRandom(Life * life) {
}

void EnergySensorEvolution::perform(Life * life) {
    InputNeuron * inputNeuron = new InputNeuron();
    inputNeuron->setName("Energy");
    inputNeuron->setGenerationNumber(this->generationNumber);

    Sensor * energySensor = new SelfEnergySensor(life->getEntity());
    energySensor->setConnectedNeuron(inputNeuron);

    life->getBrain()->addInputNeuron(inputNeuron);

    life->addSensor(energySensor);
}

std::string EnergySensorEvolution::describe() {
    return std::to_string(this->generationNumber) + " => Energy sensor";
}

Evolution * EnergySensorEvolution::generateWithMate(Evolution * mate) {
    EnergySensorEvolution * childEvolution = new EnergySensorEvolution();
    childEvolution->setGenerationNumber(getGenerationNumber());
    return childEvolution;
}
