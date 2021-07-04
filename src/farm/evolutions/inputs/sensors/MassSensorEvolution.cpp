//
// Created by Amalric Lombard de Buffières on 10/16/20.
//

#include "MassSensorEvolution.h"
#include "../../../life/sensors/SelfMassSensor.h"

MassSensorEvolution::MassSensorEvolution(): Evolution() {

}

void MassSensorEvolution::generateFromRandom(Life * life) {
}

void MassSensorEvolution::perform(Life * life) {
    InputNeuron * inputNeuron = new InputNeuron();
    inputNeuron->setName("Mass");
    inputNeuron->setGenerationNumber(this->generationNumber);

    Sensor * energySensor = new SelfMassSensor(life->getEntity(), life->getEnergyCenter());
    energySensor->setConnectedNeuron(inputNeuron);

    life->getBrain()->addInputNeuron(inputNeuron);

    life->addSensor(energySensor);
}

std::string MassSensorEvolution::describe() {
    return std::to_string(this->generationNumber) + " => Energy sensor";
}

Evolution * MassSensorEvolution::generateWithMate(Evolution * mate) {
    MassSensorEvolution * childEvolution = new MassSensorEvolution();
    childEvolution->setGenerationNumber(getGenerationNumber());
    return childEvolution;
}


std::string MassSensorEvolution::getName() {
    return "Mass sensor";
}


void MassSensorEvolution::saveInMongo(MongoClient *client, int farmId) {
    auto builder = bsoncxx::builder::stream::document{};

    builder
            << "farmId" << farmId
            << "evolution_number" << this->generationNumber
            << "type" << "mass_sensor";

    bsoncxx::document::value doc_value = builder << bsoncxx::builder::stream::finalize;


    try {
        client->saveEvolution(doc_value.view());
    } catch (const std::exception& e) {
        std::cout << "Error while saving mouth evolution: " << " in thread " << std::this_thread::get_id() << " -> " << e.what() <<  std::endl;
    }
}

bsoncxx::builder::stream::document MassSensorEvolution::generateMongoVariables() {
    auto builder = bsoncxx::builder::stream::document{};

    builder << "type" << "mass_sensor";
    return builder;
}
