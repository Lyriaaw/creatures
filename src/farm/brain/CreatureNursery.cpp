//
// Created by Amalric Lombard de Buffières on 8/2/20.
//

#include <iostream>
#include "CreatureNursery.h"
#include "BrainConnector.h"
#include "neurons/BiasNeuron.h"

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

    InputNeuron * bias = new BiasNeuron();
    bias->setName("Bias");
    brain->addInputNeuron(bias);

    OutputNeuron * speed = new SpeedOutputNeuron();
    speed->setName("Speed");
    brain->addOutputNeuron(speed);

    OutputNeuron * rotation = new RotationOutputNeuron();
    rotation->setName("Rotation");
    brain->addOutputNeuron(rotation);

    for (int it = 0; it < brain->getInputNeurons().size(); it++) {
        for (int jt = 0; jt < brain->getOutputNeurons().size(); jt++) {
            float weight = ((rand() % 200) / 100.f) - 1.f;
            Link * newLink = new Link(weight, brain->getInputNeurons().at(it), brain->getOutputNeurons().at(jt));
            brain->addLink(newLink);
        }
    }

    brain->generateLinkGrid();


    evolutionLibrary.addGenome(creature->getId(), creatureGenome);


    return connector;
}

const EvolutionLibrary &CreatureNursery::getEvolutionLibrary() const {
    return evolutionLibrary;
}
