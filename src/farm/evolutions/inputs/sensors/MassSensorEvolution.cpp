//
// Created by Amalric Lombard de Buffières on 10/16/20.
//

#include "MassSensorEvolution.h"
#include "../../../life/sensors/SelfMassSensor.h"

MassSensorEvolution::MassSensorEvolution(): Evolution() {

}

void MassSensorEvolution::generateFromRandom(Life * life) {
}

void MassSensorEvolution::perform(Life * life) {
    InputNeuron * inputNeuron = new InputNeuron();
    inputNeuron->setName("Mass");
    inputNeuron->setGenerationNumber(this->generationNumber);

    Sensor * energySensor = new SelfMassSensor(life->getEntity(), life->getEnergyCenter());
    energySensor->setConnectedNeuron(inputNeuron);

    life->getBrain()->addInputNeuron(inputNeuron);

    life->addSensor(energySensor);
}

std::string MassSensorEvolution::describe() {
    return std::to_string(this->generationNumber) + " => Energy sensor";
}

Evolution * MassSensorEvolution::generateWithMate(Evolution * mate) {
    MassSensorEvolution * childEvolution = new MassSensorEvolution();
    childEvolution->setGenerationNumber(getGenerationNumber());
    return childEvolution;
}
