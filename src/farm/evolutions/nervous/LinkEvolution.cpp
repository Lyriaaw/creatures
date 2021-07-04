//
// Created by Amalric Lombard de Buffières on 8/3/20.
//

#include <map>
#include "LinkEvolution.h"
LinkEvolution::LinkEvolution(): Evolution() {

}

void LinkEvolution::generateFromRandom(Life * life) {
}

void LinkEvolution::generateFromNeurons(Life * life, Neuron * input, Neuron * output) {
    inputGenerationNumber = input->getGenerationNumber();
    outputGenerationNumber = output->getGenerationNumber();
    weight = ((rand() % 200) / 100.f) - 1.f;
}

void LinkEvolution::setWeight(float weight) {
    LinkEvolution::weight = weight;
}

float LinkEvolution::getWeight() const {
    return weight;
}


void LinkEvolution::perform(Life * life) {

    if (!enabled) {
        return;
    }

    Neuron * inputNeuron = nullptr;
    Neuron * outputNeuron = nullptr;

    std::vector<Neuron *> connectorNeurons = life->getBrain()->getNeurons();

    for (int it = 0; it < connectorNeurons.size(); it++) {
        Neuron * currentNeuron = connectorNeurons.at(it);

        if (currentNeuron->getGenerationNumber() == inputGenerationNumber) {
            inputNeuron = currentNeuron;
        }

        if (currentNeuron->getGenerationNumber() == outputGenerationNumber) {
            outputNeuron = currentNeuron;
        }
    }

    if (inputNeuron == nullptr || outputNeuron == nullptr) {
        std::cout << "ERROR WHILE PERFORMING LINK EVOLUTION" << std::endl;
        return;
    }

    Link * newLink = new Link(weight, inputNeuron, outputNeuron);
    life->getBrain()->addLink(newLink);
}

std::string LinkEvolution::describe() {
    return std::to_string(this->generationNumber) + " => Link: " + std::to_string(inputGenerationNumber) + "->" + std::to_string(outputGenerationNumber);
}

Evolution * LinkEvolution::generateWithMate(Evolution * mate) {
    LinkEvolution * mateSensorEvolution(nullptr);


    if (mate == nullptr) {
//        std::cout << "NULL POINTER OF MATE LINK: " << generationNumber << std::endl;
    } else {
        mateSensorEvolution = dynamic_cast<LinkEvolution *>(mate);
        if (mateSensorEvolution == nullptr) {
            std::cout << "UNABLE TO CAST FATHER COLOR EVOLUTION" << std::endl;
        }
    }
    return generateFromCastedMate(mateSensorEvolution);
}

Evolution * LinkEvolution::generateFromCastedMate(LinkEvolution * mate) {
    LinkEvolution * childSensorEvolution = new LinkEvolution();
    childSensorEvolution->setGenerationNumber(getGenerationNumber());

    childSensorEvolution->inputGenerationNumber = inputGenerationNumber;
    childSensorEvolution->outputGenerationNumber = outputGenerationNumber;

    if (mate == nullptr || !isEnabled() || !mate->enabled) {
        childSensorEvolution->weight = this->weight;


        bool shouldDisableBecauseOfMate = mate != nullptr && !mate->isEnabled();
        childSensorEvolution->enabled = isEnabled() && !shouldDisableBecauseOfMate;



        return childSensorEvolution;
    }


    // 3 chances out of 4 to simply select one of the parent's sensor
    if (rand() % MUTATION_RATIO != 0) {
        if (rand() % 2 == 0) {
            childSensorEvolution->weight = this->weight;
        } else {
            childSensorEvolution->weight = mate->weight;
        }

        return childSensorEvolution;
    }

    float newWeight = (this->weight + mate->weight) / 2.f;

    childSensorEvolution->weight = newWeight;

    return childSensorEvolution;
}




std::string LinkEvolution::getName() {
    return "Link";
}

int LinkEvolution::getInputGenerationNumber() const {
    return inputGenerationNumber;
}

int LinkEvolution::getOutputGenerationNumber() const {
    return outputGenerationNumber;
}



void LinkEvolution::saveInMongo(MongoClient *client, int farmId) {
    auto builder = bsoncxx::builder::stream::document{};

    builder
            << "farmId" << farmId
            << "evolution_number" << this->generationNumber
            << "type" << "link"
            << "variables"
            << bsoncxx::builder::stream::open_document
            << "input_generation_number" << this->inputGenerationNumber
            << "output_generation_number" << this->outputGenerationNumber
            << "weight" << this->weight
            << bsoncxx::builder::stream::close_document;

    bsoncxx::document::value doc_value = builder << bsoncxx::builder::stream::finalize;


    try {
        client->saveEvolution(doc_value.view());
    } catch (const std::exception& e) {
        std::cout << "Error while saving mouth evolution: " << " in thread " << std::this_thread::get_id() << " -> " << e.what() <<  std::endl;
    }
}


bsoncxx::builder::stream::document LinkEvolution::generateMongoVariables() {
    auto builder = bsoncxx::builder::stream::document{};

    builder
            << "evolution_number" << this->generationNumber
            << "type" << "link"
            << "variables"
            << bsoncxx::builder::stream::open_document
            << "input_generation_number" << this->inputGenerationNumber
            << "output_generation_number" << this->outputGenerationNumber
            << "weight" << this->weight
            << bsoncxx::builder::stream::close_document;

    return builder;
}