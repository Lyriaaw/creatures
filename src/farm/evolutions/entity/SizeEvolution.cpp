//
// Created by Amalric Lombard de Buffières on 8/3/20.
//

#include "SizeEvolution.h"

SizeEvolution::SizeEvolution(): Evolution() {

}

void SizeEvolution::generateFromRandom(Life * life) {
    this->maxMass = (((rand() % 1000) / 100.f) + 5) * 1000.f;
}

void SizeEvolution::perform(Life * life) {
    life->getEnergyManagement()->setMaxMass(this->maxMass);
}

std::string SizeEvolution::describe() {
    return "#" +  std::to_string(this->generationNumber) + ": Max mass => " + std::to_string(this->maxMass);

}

Evolution * SizeEvolution::generateWithMate(Evolution * mate) {
    SizeEvolution * mateColorEvolution(nullptr);

    if (mate == nullptr) {
//        std::cout << "NULL POINTER OF FATHER SIZE" << std::endl;
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
        childSizeEvolution->maxMass = this->maxMass;
        return childSizeEvolution;
    }

    // 3 chances out of 4 to simply select one of the parent's sensor
    if (rand() % MUTATION_RATIO != 0) {
        if (rand() % 2 == 0) {
            childSizeEvolution->maxMass = this->maxMass;
        } else {
            childSizeEvolution->maxMass = mate->maxMass;
        }

        return childSizeEvolution;
    }

    float newSize = (this->maxMass + mate->maxMass) / 2.f;
    childSizeEvolution->maxMass = newSize;

    return childSizeEvolution;
}