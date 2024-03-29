//
// Created by Amalric Lombard de Buffières on 8/3/20.
//

#include "GenitalsEvolution.h"
#include "../../life/muscles/externals/Genitals.h"

GenitalsEvolution::GenitalsEvolution(): Evolution() {

}

void GenitalsEvolution::generateFromRandom(Life * life) {
    rotation = 1.f;
}

void GenitalsEvolution::perform(Life * life) {

    ExternalMuscle * genitals = new Genitals(rotation, life->getEntity());

    OutputNeuron * outputNeuron = new OutputNeuron();
    outputNeuron->setName("Genitals");
    outputNeuron->setGenerationNumber(this->generationNumber);
    life->getBrain()->addOutputNeuron(outputNeuron);

    genitals->addNeuron(outputNeuron);

    life->addExternalMuscle(genitals);
}

std::string GenitalsEvolution::describe() {
    return std::to_string(this->generationNumber) + " => Genitals ";
}

Evolution * GenitalsEvolution::generateWithMate(Evolution * mate) {
    GenitalsEvolution * mateColorEvolution(nullptr);

    if (mate == nullptr) {
        std::cout << "NULL POINTER OF FATHER GENITALS" << std::endl;
    } else {
        mateColorEvolution = dynamic_cast<GenitalsEvolution *>(mate);
        if (mateColorEvolution == nullptr) {
            std::cout << "UNABLE TO CAST FATHER COLOR EVOLUTION" << std::endl;
        }
    }

    return generateFromCastedMate(mateColorEvolution);
}

Evolution * GenitalsEvolution::generateFromCastedMate(GenitalsEvolution * mate) {
    GenitalsEvolution * evolution = new GenitalsEvolution();
    evolution->setGenerationNumber(getGenerationNumber());

    if (mate == nullptr) {
        evolution->rotation = this->rotation;
        return evolution;
    }

    // 3 chances out of 4 to simply select one of the parent's sensor
    if (rand() % 4 != 0) {
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