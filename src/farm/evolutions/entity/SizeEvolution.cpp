//
// Created by Amalric Lombard de Buffières on 8/3/20.
//

#include "SizeEvolution.h"

SizeEvolution::SizeEvolution(): Evolution() {

}

void SizeEvolution::generateFromRandom(Life * life) {
    this->maximumMass = (rand() % 10000);
    this->massTransformationRatio = ((rand() % 1000) / 1000.0);
}

void SizeEvolution::perform(Life * life) {
    life->setMaximumMass(this->maximumMass);
    life->setMassTransformationRatio(this->massTransformationRatio);
}

std::string SizeEvolution::describe() {
    return std::to_string(this->generationNumber) + " => Mass ";
}

Evolution * SizeEvolution::generateWithMate(Evolution * mate) {
    SizeEvolution * mateColorEvolution(nullptr);

    if (mate == nullptr) {
//        std::cout << "NULL POINTER OF FATHER SIZE" << std::endl;
    } else {
        mateColorEvolution = dynamic_cast<SizeEvolution *>(mate);
        if (mateColorEvolution == nullptr) {
            std::cout << "UNABLE TO CAST FATHER MASS EVOLUTION" << std::endl;
        }
    }

    return generateFromCastedMate(mateColorEvolution);
}

Evolution * SizeEvolution::generateFromCastedMate(SizeEvolution * mate) {
    SizeEvolution * childSizeEvolution = new SizeEvolution();
    childSizeEvolution->setGenerationNumber(getGenerationNumber());

    if (mate == nullptr) {
        childSizeEvolution->maximumMass = this->maximumMass;
        childSizeEvolution->massTransformationRatio = this->massTransformationRatio;
        return childSizeEvolution;
    }

    // 3 chances out of 4 to simply select one of the parent's sensor
    if (rand() % MUTATION_RATIO != 0) {
        if (rand() % 2 == 0) {
            childSizeEvolution->maximumMass = this->maximumMass;
            childSizeEvolution->massTransformationRatio = this->massTransformationRatio;
        } else {
            childSizeEvolution->maximumMass = mate->maximumMass;
            childSizeEvolution->massTransformationRatio = mate->massTransformationRatio;
        }

        return childSizeEvolution;
    }


    double newMaximumMass = (this->maximumMass + mate->maximumMass) / 2.f;
    childSizeEvolution->maximumMass = newMaximumMass;

    double newMassTransformationRatio = (this->massTransformationRatio + mate->massTransformationRatio) / 2.0;
    childSizeEvolution->massTransformationRatio = newMassTransformationRatio;



    return childSizeEvolution;
}