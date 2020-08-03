//
// Created by Amalric Lombard de Buffières on 8/2/20.
//

#include <iostream>
#include "CreatureNursery.h"
#include "BrainConnector.h"
#include "neurons/BiasNeuron.h"
#include "evolutions/SpeedEvolution.h"

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

    BiasInputEvolution * biasInputEvolution = new BiasInputEvolution();
    biasInputEvolution->setGenerationNumber(1);
    biasInputEvolution->perform(connector);
    creatureGenome.emplace_back(biasInputEvolution);

    int sensorCount = rand() % 7 + 4;
    for (int it = 0; it < sensorCount; it++) {
        SensorEvolution * sensorEvol = new SensorEvolution();

        sensorEvol->setGenerationNumber((it * 2) + 2);
        sensorEvol->generateFromRandom(connector);

        sensorEvol->perform(connector);

        creatureGenome.emplace_back(sensorEvol);

        std::cout << it << "Global evolution :" << Evolution::nextEvolutionNumber << std::endl;

    }

    SpeedEvolution * speedEvolution = new SpeedEvolution();
    speedEvolution->setGenerationNumber(22);
    speedEvolution->perform(connector);
    creatureGenome.emplace_back(speedEvolution);

    RotationEvolution * rotationEvolution = new RotationEvolution();
    rotationEvolution->setGenerationNumber(23);
    rotationEvolution->perform(connector);
    creatureGenome.emplace_back(rotationEvolution);


    int generationNumberIndex(24);
    for (int it = 0; it < brain->getInputNeurons().size(); it++) {
        for (int jt = 0; jt < brain->getOutputNeurons().size(); jt++) {
            LinkEvolution * linkEvolution = new LinkEvolution();
            linkEvolution->setGenerationNumber(generationNumberIndex);
            linkEvolution->generateFromNeurons(connector, brain->getInputNeurons().at(it), brain->getOutputNeurons().at(jt));
            generationNumberIndex++;
            creatureGenome.emplace_back(linkEvolution);

        }
    }

    std::cout << "Global evolution :" << Evolution::nextEvolutionNumber << std::endl;


    brain->generateLinkGrid();


    evolutionLibrary.addGenome(creature->getId(), creatureGenome);


    return connector;
}


BrainConnector * CreatureNursery::Mate(BrainConnector * father, BrainConnector * mother) {

    std::vector<Evolution *> fatherGenome = this->getEvolutionLibrary().getGenomeFor(father->getCreature()->getId());
    std::vector<Evolution *> motherGenome = this->getEvolutionLibrary().getGenomeFor(mother->getCreature()->getId());

    std::cout << "Mating " << std::endl;

}

const EvolutionLibrary &CreatureNursery::getEvolutionLibrary() const {
    return evolutionLibrary;
}
