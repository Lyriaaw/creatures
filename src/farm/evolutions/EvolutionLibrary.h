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

    int getCurrentEvolutionNumber() const;

    void setCurrentEvolutionNumber(int currentEvolutionNumber);
    void increaseEvolutionNumber();

private:
    std::vector<int> creatureIndexes;
    std::vector<std::vector<Evolution *>> genomes;

    int currentEvolutionNumber;

};


#endif //CREATURES_EVOLUTIONLIBRARY_H
