//
// Created by Amalric Lombard de Buffières on 8/2/20.
//

#ifndef CREATURES_EVOLUTIONLIBRARY_H
#define CREATURES_EVOLUTIONLIBRARY_H
#include <vector>
#include <map>
#include "Evolution.h"


class EvolutionLibrary {
private:
    MongoClient * mongoClient;
    int farmId;

    std::mutex genomes_mutex;

    std::vector<int> creatureIndexes;
    std::vector<std::vector<Evolution *>> genomes;

    std::map<int, Evolution *> allEvolutions;

    int currentEvolutionNumber;

    std::vector<std::string> events;

public:
    EvolutionLibrary(MongoClient * mongoClient, int farmId);

    void addGenome(int farmId, int creatureIndex, std::vector<Evolution *> genome);
    std::vector<Evolution *> getGenomeFor(int creatureIndex) const;

    int getCurrentEvolutionNumber() const;

    void setCurrentEvolutionNumber(int currentEvolutionNumber);
    void increaseEvolutionNumber();

    const std::map<int, Evolution *> &getAllEvolutions() const;
    void addEvolution(Evolution *evolution);

    void recordEvent(std::string event);

    const std::vector<std::string> &getEvents() const;


    void saveGenomeToMongo(int farmId, int creatureIndex, std::vector<Evolution *> genome, MongoClient * mongoClient);
};


#endif //CREATURES_EVOLUTIONLIBRARY_H
