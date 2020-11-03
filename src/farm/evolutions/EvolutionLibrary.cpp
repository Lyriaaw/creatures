//
// Created by Amalric Lombard de Buffières on 8/2/20.
//

#include <iostream>
#include "EvolutionLibrary.h"


void EvolutionLibrary::addGenome(int creatureIndex, std::vector<Evolution *> genome) {
    std::lock_guard<std::mutex> guard(genomes_mutex);

    creatureIndexes.emplace_back(creatureIndex);
    genomes.emplace_back(genome);
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
    }

}

void EvolutionLibrary::recordEvent(std::string event) {
    events.emplace_back(event);
}

const std::vector<std::string> &EvolutionLibrary::getEvents() const {
    return events;
}

EvolutionLibrary::EvolutionLibrary() {}
