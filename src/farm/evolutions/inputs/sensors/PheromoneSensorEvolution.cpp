//
// Created by Amalric Lombard de Buffières on 10/20/20.
//

#include "PheromoneSensorEvolution.h"
#include "../../../life/sensors/PheromoneSensor.h"

PheromoneSensorEvolution::PheromoneSensorEvolution(): Evolution() {

}

void PheromoneSensorEvolution::generateFromRandom(Life * life) {
    color = ((rand() % 999) / 1000.f);
}

void PheromoneSensorEvolution::perform(Life * life) {
    InputNeuron * inputNeuron = new InputNeuron();
    inputNeuron->setName("Pheromone sensor");
    inputNeuron->setHueOutline(color);
    inputNeuron->setBrightnessOutline(0.5);
    inputNeuron->setGenerationNumber(this->generationNumber);

    PheromoneSensor * pheromoneSensor = new PheromoneSensor(life->getEntity());
    pheromoneSensor->setColor(color);
    pheromoneSensor->setConnectedNeuron(inputNeuron);

    life->getBrain()->addInputNeuron(inputNeuron);

    life->addSensor(pheromoneSensor);
}

std::string PheromoneSensorEvolution::describe() {
    return std::to_string(this->generationNumber) + " => Pheromone sensor : " + std::to_string(color);
}

Evolution * PheromoneSensorEvolution::generateWithMate(Evolution * mate) {
    PheromoneSensorEvolution * childEvolution = new PheromoneSensorEvolution();
    childEvolution->setGenerationNumber(getGenerationNumber());
    childEvolution->color = color;
    return childEvolution;
}


std::string PheromoneSensorEvolution::getName() {
    return "Pheromone sensor";
}


void PheromoneSensorEvolution::saveInMongo(MongoClient *client, int farmId) {
    auto builder = bsoncxx::builder::stream::document{};

    builder
            << "farmId" << farmId
            << "evolution_number" << this->generationNumber
            << "type" << "pheromone_sensor"
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

bsoncxx::builder::stream::document PheromoneSensorEvolution::generateMongoVariables() {
    auto builder = bsoncxx::builder::stream::document{};

    builder
            << "evolution_number" << this->generationNumber
            << "type" << "color"
            << "variables"
            << bsoncxx::builder::stream::open_document
            << "color" << this->color
            << bsoncxx::builder::stream::close_document;

    return builder;
}
