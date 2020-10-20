//
// Created by Amalric Lombard de Buffières on 10/20/20.
//

#include "PheromoneEmitterEvolution.h"
#include "../../life/muscles/externals/PheromoneEmitter.h"


PheromoneEmitterEvolution::PheromoneEmitterEvolution(): Evolution(), color(0.0) {

}

void PheromoneEmitterEvolution::generateFromRandom(Life * life) {
    color = ((rand() % 999) / 1000.f);
}



void PheromoneEmitterEvolution::perform(Life * life) {
    ExternalMuscle * pheromoneEmitter = new PheromoneEmitter(0.0, life->getEntity());

    OutputNeuron * outputNeuron = new OutputNeuron();
    outputNeuron->setName("Pheromone emitter");
    outputNeuron->setHueOutline(color);
    outputNeuron->setBrightnessOutline(0.5);
    outputNeuron->setGenerationNumber(this->generationNumber);
    life->getBrain()->addOutputNeuron(outputNeuron);

    pheromoneEmitter->addNeuron(outputNeuron);

    life->addExternalMuscle(pheromoneEmitter);
}

std::string PheromoneEmitterEvolution::describe() {
    return std::to_string(this->generationNumber) + " => Pheromone emitter, color: " + std::to_string(color);
}




Evolution * PheromoneEmitterEvolution::generateWithMate(Evolution * mate) {
    PheromoneEmitterEvolution * mateSensorEvolution(nullptr);


    if (mate == nullptr) {
//        std::cout << "NULL POINTER OF MATE LINK: " << generationNumber << std::endl;
    } else {
        mateSensorEvolution = dynamic_cast<PheromoneEmitterEvolution *>(mate);
        if (mateSensorEvolution == nullptr) {
            std::cout << "UNABLE TO CAST FATHER COLOR EVOLUTION" << std::endl;
        }
    }
    return generateFromCastedMate(mateSensorEvolution);
}

Evolution * PheromoneEmitterEvolution::generateFromCastedMate(PheromoneEmitterEvolution * mate) {
    PheromoneEmitterEvolution * childSensorEvolution = new PheromoneEmitterEvolution();
    childSensorEvolution->setGenerationNumber(getGenerationNumber());

    childSensorEvolution->color = color;

    return childSensorEvolution;
}

std::string PheromoneEmitterEvolution::getName() {
    return "Pheromone emitter";
}

