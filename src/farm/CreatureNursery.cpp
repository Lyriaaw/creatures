//
// Created by Amalric Lombard de Buffières on 8/2/20.
//

#include <iostream>
#include "CreatureNursery.h"
#include "brain/BrainConnector.h"
#include "brain/neurons/BiasNeuron.h"
#include "evolutions/musclesEvolutions/SpeedEvolution.h"
#include "evolutions/nervous/DistanceBarSensorEvolution.h"

using namespace std;


CreatureNursery::CreatureNursery(): evolutionLibrary(EvolutionLibrary()){
}

Life * CreatureNursery::generateCreatureFromRandom() {
    random_device rd;
    mt19937 mt(rd());
    uniform_real_distribution<double> distWidth(0, FARM_WIDTH);
    uniform_real_distribution<double> distHeight(0, FARM_HEIGHT);

    int x = distWidth(mt);
    int y = distHeight(mt);

    Entity * entity = new Entity(Point(x, y));
    entity->setBrightness(0.5f);
    Brain * brain = new Brain();
    EnergyCenter * energyCenter = new EnergyCenter(entity);

    Life * life = new Life();
    life->setEntity(entity);
    life->setBrain(brain);
    life->setEntity(entity);
    life->setEnergyCenter(energyCenter);

    std::vector<Evolution *> creatureGenome;

    ColorEvolution * colorEvolution = new ColorEvolution();
    colorEvolution->setGenerationNumber(1);
    colorEvolution->generateFromRandom(life);
    colorEvolution->perform(life);
    creatureGenome.emplace_back(colorEvolution);

    SizeEvolution * sizeEvolution = new SizeEvolution();
    sizeEvolution->setGenerationNumber(2);
    sizeEvolution->generateFromRandom(life);
    sizeEvolution->perform(life);
    creatureGenome.emplace_back(sizeEvolution);



    BiasInputEvolution * biasInputEvolution = new BiasInputEvolution();
    biasInputEvolution->setGenerationNumber(3);
    biasInputEvolution->perform(life);
    creatureGenome.emplace_back(biasInputEvolution);

    EnergySensorEvolution * energySensorInputEvolution = new EnergySensorEvolution();
    energySensorInputEvolution->setGenerationNumber(4);
    energySensorInputEvolution->perform(life);
    creatureGenome.emplace_back(energySensorInputEvolution);



    for (int it = 0; it < 2; it++) {
        BrightnessBarSensorEvolution * sensorEvol = new BrightnessBarSensorEvolution();
        sensorEvol->setGenerationNumber(it + 5);
        sensorEvol->generateFromRandom(life);
        sensorEvol->perform(life);
        creatureGenome.emplace_back(sensorEvol);
    }

    int sensorCount = rand() % 4 + 1;
    for (int it = 0; it < sensorCount; it++) {

        DistanceBarSensorEvolution * sensorEvol = new DistanceBarSensorEvolution();
        sensorEvol->setGenerationNumber(it + 10);
        sensorEvol->generateFromRandom(life);
        sensorEvol->perform(life);
        creatureGenome.emplace_back(sensorEvol);

    }



    SpeedEvolution * speedEvolution = new SpeedEvolution();
    speedEvolution->setGenerationNumber(20);
    speedEvolution->perform(life);
    creatureGenome.emplace_back(speedEvolution);

    RotationEvolution * rotationEvolution = new RotationEvolution();
    rotationEvolution->setGenerationNumber(21);
    rotationEvolution->perform(life);
    creatureGenome.emplace_back(rotationEvolution);

    MouthEvolution * mouthEvolution = new MouthEvolution();
    mouthEvolution->setGenerationNumber(22);
    mouthEvolution->generateFromRandom(life);
    mouthEvolution->perform(life);
    creatureGenome.emplace_back(mouthEvolution);

    GenitalsEvolution * genitalsEvolution = new GenitalsEvolution();
    genitalsEvolution->setGenerationNumber(23);
    genitalsEvolution->generateFromRandom(life);
    genitalsEvolution->perform(life);
    creatureGenome.emplace_back(genitalsEvolution);






    int generationNumberIndex(30);
    for (int it = 0; it < brain->getInputNeurons().size(); it++) {
        for (int jt = 0; jt < brain->getOutputNeurons().size(); jt++) {

            LinkEvolution * linkEvolution = new LinkEvolution();
            linkEvolution->setGenerationNumber(generationNumberIndex + (jt * 10) + it);
            linkEvolution->generateFromNeurons(life, brain->getInputNeurons().at(it), brain->getOutputNeurons().at(jt));
            linkEvolution->perform(life);
            creatureGenome.emplace_back(linkEvolution);

        }
    }


    brain->generateLinkGrid();

    life->connectSensorAndMuscles();

    evolutionLibrary.addGenome(life->getEntity()->getId(), creatureGenome);
    return life;
}


Life * CreatureNursery::Mate(Life * father, Life * mother) {

    std::vector<Evolution *> fatherGenome = this->getEvolutionLibrary().getGenomeFor(father->getEntity()->getId());
    std::vector<Evolution *> motherGenome = this->getEvolutionLibrary().getGenomeFor(mother->getEntity()->getId());

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

    float delta = (((rand() % 400) / 100.f) - 2.f) * father->getEntity()->getSize();

    Point childSpawn = {
    father->getEntity()->getPosition().getX() + delta,
    father->getEntity()->getPosition().getY() + delta,
    };

    if (childSpawn.getX() < 0) {
        childSpawn.setX(0);
    }
    if (childSpawn.getX() >= FARM_WIDTH) {
        childSpawn.setX(FARM_WIDTH - 1);
    }

    if (childSpawn.getY() < 0) {
        childSpawn.setY(0);
    }
    if (childSpawn.getY() >= FARM_HEIGHT) {
        childSpawn.setY(FARM_HEIGHT - 1);
    }




    Entity * childEntity = new Entity(childSpawn);
    childEntity->setRotation(((std::rand() % 2000) / 1000.f) - 1.f);
    childEntity->setBrightness(0.5);

    Brain * childBrain = new Brain();

    EnergyCenter * energyCenter = new EnergyCenter(childEntity);

    Life * life = new Life();
    life->setEntity(childEntity);
    life->setBrain(childBrain);
    life->setEnergyCenter(energyCenter);


    for (int it = 0; it < childGenome.size(); it++) {
        childGenome.at(it)->perform(life);
    }

    life->getBrain()->generateLinkGrid();
    evolutionLibrary.addGenome(life->getEntity()->getId(), childGenome);

    life->connectSensorAndMuscles();
    return life;
}

const EvolutionLibrary &CreatureNursery::getEvolutionLibrary() const {
    return evolutionLibrary;
}
