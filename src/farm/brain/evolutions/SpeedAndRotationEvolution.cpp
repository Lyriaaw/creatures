//
// Created by Amalric Lombard de Buffières on 8/3/20.
//

#include "SpeedAndRotationEvolution.h"

SpeedAndRotationEvolution::SpeedAndRotationEvolution(): Evolution() {

}

void SpeedAndRotationEvolution::generateFromRandom(BrainConnector * connector) {
}

void SpeedAndRotationEvolution::perform(BrainConnector * connector) {
    OutputNeuron * speed = new SpeedOutputNeuron();
    speed->setName("Speed");
    connector->getBrain()->addOutputNeuron(speed);

    OutputNeuron * rotation = new RotationOutputNeuron();
    rotation->setName("Rotation");
    connector->getBrain()->addOutputNeuron(rotation);
}

void SpeedAndRotationEvolution::describe() {
    std::cout << this->generationNumber << " => Speed and rotation from brain " << std::endl;
}