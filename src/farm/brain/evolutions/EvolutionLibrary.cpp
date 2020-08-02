//
// Created by Amalric Lombard de Buffières on 8/2/20.
//

#include <iostream>
#include "EvolutionLibrary.h"


void EvolutionLibrary::addGenome(int creatureIndex, std::vector<Evolution *> genome) {
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
}