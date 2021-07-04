//
// Created by Amalric Lombard de Buffières on 8/3/20.
//

#include "SpeedEvolution.h"
#include "../../life/muscles/internals/Movement.h"

bsoncxx::builder::stream::document generateMongoVariables();

SpeedEvolution::SpeedEvolution(): Evolution() {

}

void SpeedEvolution::generateFromRandom(Life * life) {
}

void SpeedEvolution::perform(Life * life) {
    InternalMuscle * movement = new Movement(0.0, life->getEntity());

    OutputNeuron * outputNeuron = new OutputNeuron();
    outputNeuron->setName("Movement");
    outputNeuron->setGenerationNumber(this->generationNumber);
    life->getBrain()->addOutputNeuron(outputNeuron);

    movement->addNeuron(outputNeuron);

    life->addInternalMuscle(movement);
}

std::string SpeedEvolution::describe() {
    return std::to_string(this->generationNumber) + " => Speed output ";
}

Evolution * SpeedEvolution::generateWithMate(Evolution * mate) {
    SpeedEvolution * childEvolution = new SpeedEvolution();
    childEvolution->setGenerationNumber(getGenerationNumber());
    return childEvolution;
}


std::string SpeedEvolution::getName() {
    return "Speed";
}


void SpeedEvolution::saveInMongo(MongoClient *client, int farmId) {
    auto builder = bsoncxx::builder::stream::document{};

    builder
            << "farmId" << farmId
            << "evolution_number" << this->generationNumber
            << "type" << "speed";

    bsoncxx::document::value doc_value = builder << bsoncxx::builder::stream::finalize;


    try {
        client->saveEvolution(doc_value.view());
    } catch (const std::exception& e) {
        std::cout << "Error while saving mouth evolution: " << " in thread " << std::this_thread::get_id() << " -> " << e.what() <<  std::endl;
    }
}

bsoncxx::builder::stream::document SpeedEvolution::generateMongoVariables() {
    auto builder = bsoncxx::builder::stream::document{};

    builder
            << "evolution_number" << this->generationNumber
            << "type" << "speed";

    return builder;
}