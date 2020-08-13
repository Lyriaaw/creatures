//
// Created by Amalric Lombard de Buffières on 8/12/20.
//

#include "TileHeatEvolution.h"
#include "../../life/sensors/TileHeatSensor.h"

TileHeatEvolution::TileHeatEvolution() {}

void TileHeatEvolution::generateFromRandom(Life *life) {

}

void TileHeatEvolution::perform(Life *life) {
    InputNeuron * inputNeuron = new InputNeuron();
    inputNeuron->setName("Accessible heat");
    inputNeuron->setGenerationNumber(this->generationNumber);

    Sensor * sensor = new TileHeatSensor(life->getEntity());
    sensor->setConnectedNeuron(inputNeuron);

    life->getBrain()->addInputNeuron(inputNeuron);

    life->addSensor(sensor);
}

std::string TileHeatEvolution::describe() {
    return std::to_string(this->generationNumber) + " => Accessible heat energy evolution";}

Evolution *TileHeatEvolution::generateWithMate(Evolution *mate) {
    TileHeatEvolution * childEvolution = new TileHeatEvolution();
    childEvolution->setGenerationNumber(getGenerationNumber());
    return childEvolution;
}

