//
// Created by Amalric Lombard de Buffières on 8/13/20.
//

#include "CaptureGroundMuscleEvolution.h"
#include "../../life/muscles/externals/CaptureGroundMuscle.h"

CaptureGroundMuscleEvolution::CaptureGroundMuscleEvolution(): Evolution() {

}

void CaptureGroundMuscleEvolution::generateFromRandom(Life * life) {
    rotation = ((rand() % 200) / 100.f) - 1.f ;
}

void CaptureGroundMuscleEvolution::perform(Life * life) {
    ExternalMuscle * mouth = new CaptureGroundMuscle(rotation, life->getEntity());

    OutputNeuron * outputNeuron = new OutputNeuron();
    outputNeuron->setName("Capture Ground");
    outputNeuron->setGenerationNumber(this->generationNumber);
    life->getBrain()->addOutputNeuron(outputNeuron);

    mouth->addNeuron(outputNeuron);

    life->addExternalMuscle(mouth);
}

std::string CaptureGroundMuscleEvolution::describe() {
    return "#" +  std::to_string(this->generationNumber) + ": Capture Ground";
}

Evolution * CaptureGroundMuscleEvolution::generateWithMate(Evolution * mate) {
    CaptureGroundMuscleEvolution * childEvolution = new CaptureGroundMuscleEvolution();
    childEvolution->setGenerationNumber(getGenerationNumber());
    return childEvolution;
}
