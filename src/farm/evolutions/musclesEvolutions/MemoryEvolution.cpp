//
// Created by Amalric Lombard de Buffières on 10/16/20.
//

#include "MemoryEvolution.h"
#include "../../life/muscles/internals/MemoryMuscle.h"
#include "../../life/sensors/MemorySensor.h"

MemoryEvolution::MemoryEvolution(): Evolution() {

}

void MemoryEvolution::generateFromRandom(Life * life) {
    dataCount = 2;
}

void MemoryEvolution::perform(Life * life) {
    MemoryMuscle * memoryMuscle = new MemoryMuscle(life->getEntity());
    Sensor * memorySensor = new MemorySensor(life->getEntity(), memoryMuscle);

    int currentGenerationNumber = this->getGenerationNumber();

    InputNeuron * inputNeuron = new InputNeuron();
    inputNeuron->setName("MemorySensor");
    inputNeuron->setGenerationNumber(currentGenerationNumber);
    life->getBrain()->addInputNeuron(inputNeuron);
    currentGenerationNumber++;
    memorySensor->setConnectedNeuron(inputNeuron);

    OutputNeuron * outputNeuron = new OutputNeuron();
    outputNeuron->setName("MemoryMuscle");
    outputNeuron->setGenerationNumber(currentGenerationNumber);
    life->getBrain()->addOutputNeuron(outputNeuron);
    memoryMuscle->addNeuron(outputNeuron);

    life->addInternalMuscle(memoryMuscle);
    life->addSensor(memorySensor);
}

std::string MemoryEvolution::describe() {
    return std::to_string(this->generationNumber) + " => Memory ";
}

Evolution * MemoryEvolution::generateWithMate(Evolution * mate) {
    MemoryEvolution * childEvolution = new MemoryEvolution();
    childEvolution->setGenerationNumber(getGenerationNumber());
    return childEvolution;
}


std::string MemoryEvolution::getName() {
    return "Memory";
}


void MemoryEvolution::saveInMongo(MongoClient *client, int farmId) {
    auto builder = bsoncxx::builder::stream::document{};

    builder
            << "farmId" << farmId
            << "evolution_number" << this->generationNumber
            << "type" << "memory"
            << "variables"
            << bsoncxx::builder::stream::open_document
            << "data_count" << this->dataCount
            << bsoncxx::builder::stream::close_document;

    bsoncxx::document::value doc_value = builder << bsoncxx::builder::stream::finalize;


    try {
        client->saveEvolution(doc_value.view());
    } catch (const std::exception& e) {
        std::cout << "Error while saving mouth evolution: " << " in thread " << std::this_thread::get_id() << " -> " << e.what() <<  std::endl;
    }
}

bsoncxx::builder::stream::document MemoryEvolution::generateMongoVariables() {
    auto builder = bsoncxx::builder::stream::document{};

    builder
            << "evolution_number" << this->generationNumber
            << "type" << "memory"
            << "variables"
            << bsoncxx::builder::stream::open_document
            << "data_count" << this->dataCount
            << bsoncxx::builder::stream::close_document;

    return builder;
}
