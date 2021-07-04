//
// Created by Amalric Lombard de Buffières on 8/3/20.
//

#include "BiasInputEvolution.h"
#include "../../brain/neurons/BiasNeuron.h"
#include "../../life/sensors/BiasSensor.h"

BiasInputEvolution::BiasInputEvolution(): Evolution() {

}

void BiasInputEvolution::generateFromRandom(Life * life) {
}

void BiasInputEvolution::perform(Life * life) {
    InputNeuron * inputNeuron = new InputNeuron();
    inputNeuron->setName("Bias");
    inputNeuron->setGenerationNumber(this->generationNumber);

    Sensor * biasSensor = new BiasSensor(life->getEntity());
    biasSensor->setConnectedNeuron(inputNeuron);

    life->getBrain()->addInputNeuron(inputNeuron);

    life->addSensor(biasSensor);
}

std::string BiasInputEvolution::describe() {
    return std::to_string(this->generationNumber) + " => Bias Input";
}

Evolution * BiasInputEvolution::generateWithMate(Evolution * mate) {
    BiasInputEvolution * childEvolution = new BiasInputEvolution();
    childEvolution->setGenerationNumber(getGenerationNumber());
    return childEvolution;
}


std::string BiasInputEvolution::getName() {
    return "Bias input";
}


void BiasInputEvolution::saveInMongo(MongoClient *client, int farmId) {
    auto builder = bsoncxx::builder::stream::document{};

    builder
            << "farmId" << farmId
            << "evolution_number" << this->generationNumber
            << "type" << "bias_sensor";

    bsoncxx::document::value doc_value = builder << bsoncxx::builder::stream::finalize;


    try {
        client->saveEvolution(doc_value.view());
    } catch (const std::exception& e) {
        std::cout << "Error while saving mouth evolution: " << " in thread " << std::this_thread::get_id() << " -> " << e.what() <<  std::endl;
    }
}

bsoncxx::builder::stream::document BiasInputEvolution::generateMongoVariables() {
    auto builder = bsoncxx::builder::stream::document{};

    builder
            << "evolution_number" << this->generationNumber
            << "type" << "bias_sensor";

    return builder;
}
