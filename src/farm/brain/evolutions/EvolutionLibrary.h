//
// Created by Amalric Lombard de Buffières on 8/2/20.
//

#ifndef CREATURES_EVOLUTIONLIBRARY_H
#define CREATURES_EVOLUTIONLIBRARY_H
#include <vector>
#include "Evolution.h"


class EvolutionLibrary {
public:
    void addGenome(int creatureIndex, std::vector<Evolution *> genome);
    std::vector<Evolution *> getGenomeFor(int creatureIndex) const;
private:
    std::vector<int> creatureIndexes;
    std::vector<std::vector<Evolution *>> genomes;
};


#endif //CREATURES_EVOLUTIONLIBRARY_H
