//
// Created by Amalric Lombard de Buffières on 8/12/20.
//

#include "AccessibleGroundEnergyEvolution.h"
#include "../../life/sensors/AccessibleGroundEnergySensor.h"

AccessibleGroundEnergyEvolution::AccessibleGroundEnergyEvolution() {}

void AccessibleGroundEnergyEvolution::generateFromRandom(Life *life) {

}

void AccessibleGroundEnergyEvolution::perform(Life *life) {
    InputNeuron * inputNeuron = new InputNeuron();
    inputNeuron->setName("AccessibleGround");
    inputNeuron->setGenerationNumber(this->generationNumber);

    Sensor * sensor = new AccessibleGroundEnergySensor(life->getEntity());
    sensor->setConnectedNeuron(inputNeuron);

    life->getBrain()->addInputNeuron(inputNeuron);

    life->addSensor(sensor);
}

std::string AccessibleGroundEnergyEvolution::describe() {
    return std::to_string(this->generationNumber) + " => Accessible ground energy evolution";}

Evolution *AccessibleGroundEnergyEvolution::generateWithMate(Evolution *mate) {
    AccessibleGroundEnergyEvolution * childEvolution = new AccessibleGroundEnergyEvolution();
    childEvolution->setGenerationNumber(getGenerationNumber());
    return childEvolution;
}
