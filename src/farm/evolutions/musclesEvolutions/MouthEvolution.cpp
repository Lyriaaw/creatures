//
// Created by Amalric Lombard de Buffières on 8/3/20.
//

#include "MouthEvolution.h"
#include "../../life/muscles/externals/Mouth.h"

MouthEvolution::MouthEvolution(): Evolution() {

}

void MouthEvolution::generateFromRandom(Life * life) {
//    rotation = ((rand() % 200) / 100.f) - 1.f ;
    rotation = 0;
}

void MouthEvolution::perform(Life * life) {
    ExternalMuscle * mouth = new Mouth(rotation, life->getEntity());

    OutputNeuron * outputNeuron = new OutputNeuron();
    outputNeuron->setName("Mouth");
    outputNeuron->setGenerationNumber(this->generationNumber);
    life->getBrain()->addOutputNeuron(outputNeuron);

    mouth->addNeuron(outputNeuron);

    life->addExternalMuscle(mouth);
}

std::string MouthEvolution::describe() {
    return std::to_string(this->generationNumber) + " => Mouth ";
}

Evolution * MouthEvolution::generateWithMate(Evolution * mate) {
    MouthEvolution * mateColorEvolution(nullptr);

    if (mate == nullptr) {
        std::cout << "NULL POINTER OF FATHER MOUTH" << std::endl;
    } else {
        mateColorEvolution = dynamic_cast<MouthEvolution *>(mate);
        if (mateColorEvolution == nullptr) {
            std::cout << "UNABLE TO CAST FATHER COLOR EVOLUTION" << std::endl;
        }
    }

    return generateFromCastedMate(mateColorEvolution);
}

Evolution * MouthEvolution::generateFromCastedMate(MouthEvolution * mate) {
    MouthEvolution * evolution = new MouthEvolution();
    evolution->setGenerationNumber(getGenerationNumber());

    if (mate == nullptr) {
        evolution->rotation = this->rotation;
        return evolution;
    }

    // 3 chances out of 4 to simply select one of the parent's sensor
    if (rand() % MUTATION_RATIO != 0) {
        if (rand() % 2 == 0) {
            evolution->rotation = this->rotation;
        } else {
            evolution->rotation = mate->rotation;
        }

        return evolution;
    }

    float newRotation;
    float smallestRotation = (this->rotation + 1) / 2.f;
    if ((mate->rotation + 1) / 2.f < smallestRotation) {
        smallestRotation = (mate->rotation + 1) / 2.f;
    }

    float rawRotationDistance =  abs((this->rotation + 1) / 2.f - (mate->rotation + 1) / 2.f);
    float rotationDistances = std::min(rawRotationDistance, 1.f - rawRotationDistance);

    if (rawRotationDistance < 1 - rawRotationDistance) {
        newRotation = smallestRotation + (rotationDistances / 2.f);
    } else {
        newRotation = 1 + (smallestRotation - (rotationDistances / 2.f));
    }

    newRotation = (newRotation * 2.f) - 1.f;

    evolution->rotation = newRotation;

    return evolution;
}