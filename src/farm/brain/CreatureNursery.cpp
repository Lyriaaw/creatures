//
// Created by Amalric Lombard de Buffières on 8/2/20.
//

#include <iostream>
#include "CreatureNursery.h"
#include "BrainConnector.h"

using namespace std;


CreatureNursery::CreatureNursery(): evolutionLibrary(EvolutionLibrary()){
}

BrainConnector * CreatureNursery::generateFromRandom() {
    random_device rd;
    mt19937 mt(rd());
    uniform_real_distribution<double> distWidth(0, FARM_WIDTH);
    uniform_real_distribution<double> distHeight(0, FARM_HEIGHT);

    int x = distWidth(mt);
    int y = distHeight(mt);

    Creature * creature = new Creature(Point(x, y), 10);
    Brain * brain = new Brain();

    BrainConnector * connector = new BrainConnector(creature, brain);

    std::vector<Evolution *> creatureGenome;
    int sensorCount = rand() % 7 + 3;
    for (int it = 0; it < sensorCount; it++) {
        SensorEvolution * sensorEvol = new SensorEvolution();

        sensorEvol->setGenerationNumber(it);
        sensorEvol->generateFromRandom(connector);

        sensorEvol->perform(connector);

        creatureGenome.emplace_back(sensorEvol);
    }

    evolutionLibrary.addGenome(creature->getId(), creatureGenome);


    return connector;
}

const EvolutionLibrary &CreatureNursery::getEvolutionLibrary() const {
    return evolutionLibrary;
}
