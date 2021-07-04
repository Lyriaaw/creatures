//
// Created by Amalric Lombard de Buffières on 8/5/20.
//

#include "EnergySensorEvolution.h"
#include "../../../brain/neurons/EnergySensorNeuron.h"
#include "../../../life/sensors/SelfEnergySensor.h"

EnergySensorEvolution::EnergySensorEvolution(): Evolution() {

}

void EnergySensorEvolution::generateFromRandom(Life * life) {
}

void EnergySensorEvolution::perform(Life * life) {
    InputNeuron * inputNeuron = new InputNeuron();
    inputNeuron->setName("Energy");
    inputNeuron->setGenerationNumber(this->generationNumber);

    Sensor * energySensor = new SelfEnergySensor(life->getEntity(), life->getEnergyCenter());
    energySensor->setConnectedNeuron(inputNeuron);

    life->getBrain()->addInputNeuron(inputNeuron);

    life->addSensor(energySensor);
}

std::string EnergySensorEvolution::describe() {
    return std::to_string(this->generationNumber) + " => Energy sensor";
}

Evolution * EnergySensorEvolution::generateWithMate(Evolution * mate) {
    EnergySensorEvolution * childEvolution = new EnergySensorEvolution();
    childEvolution->setGenerationNumber(getGenerationNumber());
    return childEvolution;
}


std::string EnergySensorEvolution::getName() {
    return "Available Energy Sensor";
}


void EnergySensorEvolution::saveInMongo(MongoClient *client, int farmId) {
    auto builder = bsoncxx::builder::stream::document{};

    builder
            << "farmId" << farmId
            << "evolution_number" << this->generationNumber
            << "type" << "energy_sensor";

    bsoncxx::document::value doc_value = builder << bsoncxx::builder::stream::finalize;


    try {
        client->saveEvolution(doc_value.view());
    } catch (const std::exception& e) {
        std::cout << "Error while saving mouth evolution: " << " in thread " << std::this_thread::get_id() << " -> " << e.what() <<  std::endl;
    }
}


bsoncxx::builder::stream::document EnergySensorEvolution::generateMongoVariables() {
    auto builder = bsoncxx::builder::stream::document{};

    builder << "type" << "energy_sensor";
    return builder;
}