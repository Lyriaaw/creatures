//
// Created by Amalric Lombard de Buffières on 8/3/20.
//

#include "RotationEvolution.h"
#include "../../life/muscles/internals/Rotation.h"

RotationEvolution::RotationEvolution(): Evolution() {

}

void RotationEvolution::generateFromRandom(Life * life) {
}

void RotationEvolution::perform(Life * life) {
    InternalMuscle * rotation = new Rotation(0.0, life->getEntity());

    OutputNeuron * outputNeuron = new OutputNeuron();
    outputNeuron->setName("Rotation");
    outputNeuron->setGenerationNumber(this->generationNumber);
    life->getBrain()->addOutputNeuron(outputNeuron);

    rotation->addNeuron(outputNeuron);

    life->addInternalMuscle(rotation);
}

std::string RotationEvolution::describe() {
    return std::to_string(this->generationNumber) + " => Rotation output ";
}

Evolution * RotationEvolution::generateWithMate(Evolution * mate) {
    RotationEvolution * childEvolution = new RotationEvolution();
    childEvolution->setGenerationNumber(getGenerationNumber());
    return childEvolution;
}


std::string RotationEvolution::getName() {
    return "Rotation";
}



void RotationEvolution::saveInMongo(MongoClient *client, int farmId) {
    auto builder = bsoncxx::builder::stream::document{};

    builder
            << "farmId" << farmId
            << "evolution_number" << this->generationNumber
            << "type" << "rotation";

    bsoncxx::document::value doc_value = builder << bsoncxx::builder::stream::finalize;


    try {
        client->saveEvolution(doc_value.view());
    } catch (const std::exception& e) {
        std::cout << "Error while saving mouth evolution: " << " in thread " << std::this_thread::get_id() << " -> " << e.what() <<  std::endl;
    }
}

bsoncxx::builder::stream::document RotationEvolution::generateMongoVariables() {
    auto builder = bsoncxx::builder::stream::document{};

    builder
            << "evolution_number" << this->generationNumber
            << "type" << "rotation";

    return builder;
}