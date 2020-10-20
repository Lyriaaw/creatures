//
// Created by Amalric Lombard de Buffières on 10/20/20.
//

#include "PheromoneSensorEvolution.h"
#include "../../../life/sensors/PheromoneSensor.h"

PheromoneSensorEvolution::PheromoneSensorEvolution(): Evolution() {

}

void PheromoneSensorEvolution::generateFromRandom(Life * life) {
    color = ((rand() % 999) / 1000.f);
}

void PheromoneSensorEvolution::perform(Life * life) {
    InputNeuron * inputNeuron = new InputNeuron();
    inputNeuron->setName("Pheromone sensor");
    inputNeuron->setHueOutline(color);
    inputNeuron->setBrightnessOutline(0.5);
    inputNeuron->setGenerationNumber(this->generationNumber);

    PheromoneSensor * pheromoneSensor = new PheromoneSensor(life->getEntity());
    pheromoneSensor->setColor(color);
    pheromoneSensor->setConnectedNeuron(inputNeuron);

    life->getBrain()->addInputNeuron(inputNeuron);

    life->addSensor(pheromoneSensor);
}

std::string PheromoneSensorEvolution::describe() {
    return std::to_string(this->generationNumber) + " => Pheromone sensor : " + std::to_string(color);
}

Evolution * PheromoneSensorEvolution::generateWithMate(Evolution * mate) {
    PheromoneSensorEvolution * childEvolution = new PheromoneSensorEvolution();
    childEvolution->setGenerationNumber(getGenerationNumber());
    childEvolution->color = color;
    return childEvolution;
}


std::string PheromoneSensorEvolution::getName() {
    return "Pheromone sensor";
}
