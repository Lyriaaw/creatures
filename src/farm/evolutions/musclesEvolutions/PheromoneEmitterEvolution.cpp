//
// Created by Amalric Lombard de Buffières on 10/20/20.
//

#include "PheromoneEmitterEvolution.h"
#include "../../life/muscles/externals/PheromoneEmitter.h"


PheromoneEmitterEvolution::PheromoneEmitterEvolution(): Evolution(), color(0.0) {

}

void PheromoneEmitterEvolution::generateFromRandom(Life * life) {
    color = ((rand() % 999) / 1000.f);
}



void PheromoneEmitterEvolution::perform(Life * life) {
    PheromoneEmitter * pheromoneEmitter = new PheromoneEmitter(0.0, life->getEntity());
    pheromoneEmitter->setColor(color);

    OutputNeuron * outputNeuron = new OutputNeuron();
    outputNeuron->setName("Pheromone emitter");
    outputNeuron->setHueOutline(color);
    outputNeuron->setBrightnessOutline(0.5);
    outputNeuron->setGenerationNumber(this->generationNumber);
    life->getBrain()->addOutputNeuron(outputNeuron);

    pheromoneEmitter->addNeuron(outputNeuron);

    life->addExternalMuscle(pheromoneEmitter);
}

std::string PheromoneEmitterEvolution::describe() {
    return std::to_string(this->generationNumber) + " => Pheromone emitter, color: " + std::to_string(color);
}




Evolution * PheromoneEmitterEvolution::generateWithMate(Evolution * mate) {
    PheromoneEmitterEvolution * mateSensorEvolution(nullptr);


    if (mate == nullptr) {
//        std::cout << "NULL POINTER OF MATE LINK: " << generationNumber << std::endl;
    } else {
        mateSensorEvolution = dynamic_cast<PheromoneEmitterEvolution *>(mate);
        if (mateSensorEvolution == nullptr) {
            std::cout << "UNABLE TO CAST FATHER COLOR EVOLUTION" << std::endl;
        }
    }
    return generateFromCastedMate(mateSensorEvolution);
}

Evolution * PheromoneEmitterEvolution::generateFromCastedMate(PheromoneEmitterEvolution * mate) {
    PheromoneEmitterEvolution * childSensorEvolution = new PheromoneEmitterEvolution();
    childSensorEvolution->setGenerationNumber(getGenerationNumber());

    childSensorEvolution->color = color;

    return childSensorEvolution;
}

std::string PheromoneEmitterEvolution::getName() {
    return "Pheromone emitter";
}



void PheromoneEmitterEvolution::saveInMongo(MongoClient *client, int farmId) {
    auto builder = bsoncxx::builder::stream::document{};

    builder
            << "farmId" << farmId
            << "evolution_number" << this->generationNumber
            << "type" << "pheromone_emitter"
            << "variables"
            << bsoncxx::builder::stream::open_document
            << "color" << this->color
            << bsoncxx::builder::stream::close_document;

    bsoncxx::document::value doc_value = builder << bsoncxx::builder::stream::finalize;


    try {
        client->saveEvolution(doc_value.view());
    } catch (const std::exception& e) {
        std::cout << "Error while saving mouth evolution: " << " in thread " << std::this_thread::get_id() << " -> " << e.what() <<  std::endl;
    }
}

bsoncxx::builder::stream::document PheromoneEmitterEvolution::generateMongoVariables() {
    auto builder = bsoncxx::builder::stream::document{};

    builder
            << "evolution_number" << this->generationNumber
            << "type" << "pheromone_emitter"
            << "variables"
            << bsoncxx::builder::stream::open_document
            << "color" << this->color
            << bsoncxx::builder::stream::close_document;

    return builder;
}