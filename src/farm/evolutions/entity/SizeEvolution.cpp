//
// Created by Amalric Lombard de Buffières on 8/3/20.
//

#include "SizeEvolution.h"

SizeEvolution::SizeEvolution(): Evolution() {

}

void SizeEvolution::generateFromRandom(Life * life) {
    this->size = ((rand() % 500) / 100.f) + 2;
}

void SizeEvolution::perform(Life * life) {
    life->getEnergyCenter()->setMaxMass(this->size * MASS_TO_SIZE_RATIO);
}

std::string SizeEvolution::describe() {
    return std::to_string(this->generationNumber) + " => Size " + std::to_string(this->size);
}

Evolution * SizeEvolution::generateWithMate(Evolution * mate) {
    SizeEvolution * mateColorEvolution(nullptr);

    if (mate == nullptr) {
        std::cout << "NULL POINTER OF FATHER COLOR" << std::endl;
    } else {
        mateColorEvolution = dynamic_cast<SizeEvolution *>(mate);
        if (mateColorEvolution == nullptr) {
            std::cout << "UNABLE TO CAST FATHER COLOR EVOLUTION" << std::endl;
        }
    }

    return generateFromCastedMate(mateColorEvolution);
}

Evolution * SizeEvolution::generateFromCastedMate(SizeEvolution * mate) {
    SizeEvolution * childSizeEvolution = new SizeEvolution();
    childSizeEvolution->setGenerationNumber(getGenerationNumber());

    if (mate == nullptr) {
        childSizeEvolution->size = this->size;
        return childSizeEvolution;
    }

    // 3 chances out of 4 to simply select one of the parent's sensor
    if (rand() % MUTATION_RATIO != 0) {
        if (rand() % 2 == 0) {
            childSizeEvolution->size = this->size;
        } else {
            childSizeEvolution->size = mate->size;
        }

        return childSizeEvolution;
    }

    float newSize = (this->size + mate->size) / 2.f;
    childSizeEvolution->size = newSize;

    return childSizeEvolution;
}


std::string SizeEvolution::getName() {
    return "Size";
}
