//
// Created by Amalric Lombard de Buffières on 8/2/20.
//

#include <iostream>
#include "EvolutionLibrary.h"

EvolutionLibrary::EvolutionLibrary(MongoClient * mongoClient, int farmId): mongoClient(mongoClient), farmId(farmId) {}

void EvolutionLibrary::addGenome(int farmId, int creatureIndex, std::vector<Evolution *> genome) {
    std::lock_guard<std::mutex> guard(genomes_mutex);

    creatureIndexes.emplace_back(creatureIndex);
    genomes.emplace_back(genome);
}

void EvolutionLibrary::saveGenomeToMongo(int farmId, int creatureIndex, std::vector<Evolution *> genome, MongoClient * mongoClient) {
    auto builder = bsoncxx::builder::stream::document{};

    auto array_builder = bsoncxx::builder::basic::array{};
    for (int it = 0; it < genome.size(); it++) {
        array_builder.append(genome[it]->generateMongoVariables());
    }

    builder
            << "farmId" << farmId
            << "id" << creatureIndex
            << "genome" << array_builder;



    bsoncxx::document::value doc_value = builder << bsoncxx::builder::stream::finalize;


    try {
        mongoClient->saveGenome(doc_value.view());

    } catch (const std::exception& e) {
        std::cout << "Error while saving creature's genome: " << creatureIndex << " in thread " << std::this_thread::get_id() << " -> " << e.what() <<  std::endl;
    }
}

std::vector<Evolution *> EvolutionLibrary::getGenomeFor(int creatureIndex) const {
    for (int it = 0; it < this->creatureIndexes.size(); it++) {
        if (creatureIndexes.at(it) == creatureIndex) {
            return genomes.at(it);
        }
    }


    std::cout << "Genome not found for index " << creatureIndex << ", this is stange" << std::endl;

    std::vector<Evolution *> emptyEvolution;
    return emptyEvolution;
}

int EvolutionLibrary::getCurrentEvolutionNumber() const {
    return currentEvolutionNumber;
}

void EvolutionLibrary::setCurrentEvolutionNumber(int currentEvolutionNumber) {
    EvolutionLibrary::currentEvolutionNumber = currentEvolutionNumber;
}

void EvolutionLibrary::increaseEvolutionNumber() {
    currentEvolutionNumber++;
}

const std::map<int, Evolution *> &EvolutionLibrary::getAllEvolutions() const {
    return allEvolutions;
}

void EvolutionLibrary::addEvolution(Evolution * evolution) {
    if (allEvolutions.find(evolution->getGenerationNumber()) == allEvolutions.end()) {
        allEvolutions[evolution->getGenerationNumber()] = evolution;
        evolution->saveInMongo(mongoClient, farmId);
    }

}

void EvolutionLibrary::recordEvent(std::string event) {
    events.emplace_back(event);
}

const std::vector<std::string> &EvolutionLibrary::getEvents() const {
    return events;
}


