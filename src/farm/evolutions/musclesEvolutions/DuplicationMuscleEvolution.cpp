//
// Created by Amalric Lombard de Buffières on 8/12/20.
//

#include "DuplicationMuscleEvolution.h"
#include "../../life/muscles/externals/DuplicationMuscle.h"

DuplicationMuscleEvolution::DuplicationMuscleEvolution(): Evolution() {

}

void DuplicationMuscleEvolution::generateFromRandom(Life * life) {
    rotation = ((rand() % 200) / 100.f) - 1.f ;
}

void DuplicationMuscleEvolution::perform(Life * life) {

    ExternalMuscle * genitals = new DuplicationMuscle(rotation, life->getEntity());

    OutputNeuron * outputNeuron = new OutputNeuron();
    outputNeuron->setName("Duplication");
    outputNeuron->setGenerationNumber(this->generationNumber);
    life->getBrain()->addOutputNeuron(outputNeuron);

    genitals->addNeuron(outputNeuron);

    life->addExternalMuscle(genitals);
}

std::string DuplicationMuscleEvolution::describe() {
    return std::to_string(this->generationNumber) + " => Duplication ";
}

Evolution * DuplicationMuscleEvolution::generateWithMate(Evolution * mate) {
    DuplicationMuscleEvolution * evolution = new DuplicationMuscleEvolution();
    evolution->setGenerationNumber(getGenerationNumber());

    if (mate == nullptr) {
        evolution->rotation = this->rotation;
        return evolution;
    }

//    // 3 chances out of 4 to simply select one of the parent's sensor
//    if (rand() % 4 != 0) {
//        if (rand() % 2 == 0) {
//            evolution->rotation = this->rotation;
//        } else {
//            evolution->rotation = mate->rotation;
//        }
//
//        return evolution;
//    }
//
//    float newRotation;
//    float smallestRotation = (this->rotation + 1) / 2.f;
//    if ((mate->rotation + 1) / 2.f < smallestRotation) {
//        smallestRotation = (mate->rotation + 1) / 2.f;
//    }
//
//    float rawRotationDistance =  abs((this->rotation + 1) / 2.f - (mate->rotation + 1) / 2.f);
//    float rotationDistances = std::min(rawRotationDistance, 1.f - rawRotationDistance);
//
//    if (rawRotationDistance < 1 - rawRotationDistance) {
//        newRotation = smallestRotation + (rotationDistances / 2.f);
//    } else {
//        newRotation = 1 + (smallestRotation - (rotationDistances / 2.f));
//    }
//
//    newRotation = (newRotation * 2.f) - 1.f;
//
//    evolution->rotation = newRotation;

    return evolution;

}
