//
// Created by Amalric Lombard de Buffières on 10/16/20.
//

#include "MemoryEvolution.h"
#include "../../life/muscles/internals/MemoryMuscle.h"
#include "../../life/sensors/MemorySensor.h"

MemoryEvolution::MemoryEvolution(): Evolution() {

}

void MemoryEvolution::generateFromRandom(Life * life) {
    dataCount = 2;
}

void MemoryEvolution::perform(Life * life) {
    MemoryMuscle * memoryMuscle = new MemoryMuscle(life->getEntity());
    Sensor * memorySensor = new MemorySensor(life->getEntity(), memoryMuscle);

    int currentGenerationNumber = this->getGenerationNumber();

    InputNeuron * inputNeuron = new InputNeuron();
    inputNeuron->setName("MemorySensor");
    inputNeuron->setGenerationNumber(currentGenerationNumber);
    life->getBrain()->addInputNeuron(inputNeuron);
    currentGenerationNumber++;
    memorySensor->setConnectedNeuron(inputNeuron);

    OutputNeuron * outputNeuron = new OutputNeuron();
    outputNeuron->setName("MemoryMuscle");
    outputNeuron->setGenerationNumber(currentGenerationNumber);
    life->getBrain()->addOutputNeuron(outputNeuron);
    memoryMuscle->addNeuron(outputNeuron);

    life->addInternalMuscle(memoryMuscle);
    life->addSensor(memorySensor);
}

std::string MemoryEvolution::describe() {
    return std::to_string(this->generationNumber) + " => Memory ";
}

Evolution * MemoryEvolution::generateWithMate(Evolution * mate) {
    MemoryEvolution * childEvolution = new MemoryEvolution();
    childEvolution->setGenerationNumber(getGenerationNumber());
    return childEvolution;
}


std::string MemoryEvolution::getName() {
    return "Memory";
}

