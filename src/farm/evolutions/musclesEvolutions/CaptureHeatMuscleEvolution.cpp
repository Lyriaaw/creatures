//
// Created by Amalric Lombard de Buffières on 8/13/20.
//

#include "CaptureHeatMuscleEvolution.h"
#include "../../life/muscles/externals/CaptureHeatMuscle.h"


CaptureHeatMuscleEvolution::CaptureHeatMuscleEvolution(): Evolution() {

}

void CaptureHeatMuscleEvolution::generateFromRandom(Life * life) {
//    rotation = ((rand() % 200) / 100.f) - 1.f ;
    rotation = 0;
}

void CaptureHeatMuscleEvolution::perform(Life * life) {
    ExternalMuscle * mouth = new CaptureHeatMuscle(rotation, life->getEntity());

    OutputNeuron * outputNeuron = new OutputNeuron();
    outputNeuron->setName("Capture Heat");
    outputNeuron->setGenerationNumber(this->generationNumber);
    life->getBrain()->addOutputNeuron(outputNeuron);

    mouth->addNeuron(outputNeuron);

    life->addExternalMuscle(mouth);
}

std::string CaptureHeatMuscleEvolution::describe() {
    return std::to_string(this->generationNumber) + " => Capture heat ";
}

Evolution * CaptureHeatMuscleEvolution::generateWithMate(Evolution * mate) {
    CaptureHeatMuscleEvolution * childEvolution = new CaptureHeatMuscleEvolution();
    childEvolution->setGenerationNumber(getGenerationNumber());
    return childEvolution;
}
