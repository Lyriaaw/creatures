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

    Creature * creature = new Creature(Point(x, y));
    Brain * brain = new Brain();

    BrainConnector * connector = new BrainConnector(creature, brain);
    std::vector<Evolution *> creatureGenome;

    ColorEvolution * colorEvolution = new ColorEvolution();
    colorEvolution->setGenerationNumber(1);
    colorEvolution->generateFromRandom(connector);
    colorEvolution->perform(connector);
    creatureGenome.emplace_back(colorEvolution);

    SizeEvolution * sizeEvolution = new SizeEvolution();
    sizeEvolution->setGenerationNumber(2);
    sizeEvolution->generateFromRandom(connector);
    sizeEvolution->perform(connector);
    creatureGenome.emplace_back(sizeEvolution);



    BiasInputEvolution * biasInputEvolution = new BiasInputEvolution();
    biasInputEvolution->setGenerationNumber(3);
    biasInputEvolution->perform(connector);
    creatureGenome.emplace_back(biasInputEvolution);

    int sensorCount = rand() % 7 + 4;
    for (int it = 0; it < sensorCount; it++) {

        SensorEvolution * sensorEvol = new SensorEvolution();
        sensorEvol->setGenerationNumber((it * 2) + 4);
        sensorEvol->generateFromRandom(connector);
        sensorEvol->perform(connector);
        creatureGenome.emplace_back(sensorEvol);

    }

    SpeedEvolution * speedEvolution = new SpeedEvolution();
    speedEvolution->setGenerationNumber(24);
    speedEvolution->perform(connector);
    creatureGenome.emplace_back(speedEvolution);

    RotationEvolution * rotationEvolution = new RotationEvolution();
    rotationEvolution->setGenerationNumber(25);
    rotationEvolution->perform(connector);
    creatureGenome.emplace_back(rotationEvolution);


    int generationNumberIndex(26);
    for (int it = 0; it < brain->getInputNeurons().size(); it++) {
        for (int jt = 0; jt < brain->getOutputNeurons().size(); jt++) {

            LinkEvolution * linkEvolution = new LinkEvolution();
            linkEvolution->setGenerationNumber(generationNumberIndex);
            linkEvolution->generateFromNeurons(connector, brain->getInputNeurons().at(it), brain->getOutputNeurons().at(jt));
            linkEvolution->perform(connector);
            generationNumberIndex++;
            creatureGenome.emplace_back(linkEvolution);

        }
    }


    brain->generateLinkGrid();

    evolutionLibrary.addGenome(creature->getId(), creatureGenome);
    return connector;
}


BrainConnector * CreatureNursery::Mate(BrainConnector * father, BrainConnector * mother) {

    std::vector<Evolution *> fatherGenome = this->getEvolutionLibrary().getGenomeFor(father->getCreature()->getId());
    std::vector<Evolution *> motherGenome = this->getEvolutionLibrary().getGenomeFor(mother->getCreature()->getId());

    int maxGenerationNumber(0);


    for (auto & fatherGene : fatherGenome) {
        if (fatherGene->getGenerationNumber() > maxGenerationNumber) {
            maxGenerationNumber = fatherGene->getGenerationNumber();
        }
    }
    for (auto & motherGene : motherGenome) {
        if (motherGene->getGenerationNumber() > maxGenerationNumber) {
            maxGenerationNumber = motherGene->getGenerationNumber();
        }
    }

    std::vector<std::vector<Evolution *>> evolutionList;

    for (int it = 0; it <= maxGenerationNumber; it++) {
        evolutionList.push_back({nullptr, nullptr});
    }

    for (auto & fatherGene : fatherGenome) {
        evolutionList.at(fatherGene->getGenerationNumber()).at(0) = fatherGene;
    }
    for (auto & motherGene : motherGenome) {
        evolutionList.at(motherGene->getGenerationNumber()).at(1) = motherGene;
    }

    std::vector<Evolution *> childGenome;
    for (int it = 0; it < evolutionList.size(); it++) {
        Evolution * fatherGene = evolutionList.at(it).at(0);
        Evolution * motherGene = evolutionList.at(it).at(1);

//        std::cout << "Evolution #" << it << " => " << std::endl;
        if (fatherGene == nullptr && motherGene == nullptr) {
//            std::cout << "No parent gene found " << std::endl << std::endl;
            continue;
        }

        Evolution * childEvolution(nullptr);

        if (fatherGene != nullptr) {
            childEvolution = fatherGene->generateWithMate(motherGene);
        } else {
            childEvolution = motherGene->generateWithMate(fatherGene);
        }

        if (childEvolution != nullptr) {
            childGenome.emplace_back(childEvolution);
        }

    }

    Point childSpawn = {
    (father->getCreature()->getPosition().getX() + mother->getCreature()->getPosition().getX()) / 2,
    (father->getCreature()->getPosition().getY() + mother->getCreature()->getPosition().getY()) / 2,
    };

    Creature * childCreature = new Creature(childSpawn);
    Brain * childBrain = new Brain();
    BrainConnector * connector = new BrainConnector(childCreature, childBrain);


    for (int it = 0; it < childGenome.size(); it++) {
        childGenome.at(it)->perform(connector);
        std::cout << "Applying evolution on child: " << childGenome.at(it)->describe() << std::endl;
    }

    connector->getBrain()->generateLinkGrid();
    evolutionLibrary.addGenome(childCreature->getId(), childGenome);

    return connector;
}

const EvolutionLibrary &CreatureNursery::getEvolutionLibrary() const {
    return evolutionLibrary;
}
