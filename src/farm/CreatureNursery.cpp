//
// Created by Amalric Lombard de Buffières on 8/2/20.
//

#include <iostream>
#include "CreatureNursery.h"
#include "brain/BrainConnector.h"
#include "brain/neurons/BiasNeuron.h"
#include "evolutions/musclesEvolutions/SpeedEvolution.h"
#include "evolutions/sensors/SelfEnergySensorEvolution.h"
#include "evolutions/sensors/BarSensorDistanceEvolution.h"
#include "evolutions/sensors/AccessibleGroundEnergyEvolution.h"
#include "evolutions/sensors/TileHeatEvolution.h"
#include "evolutions/musclesEvolutions/DuplicationMuscleEvolution.h"
#include "evolutions/musclesEvolutions/CaptureGroundMuscleEvolution.h"
#include "evolutions/musclesEvolutions/CaptureHeatMuscleEvolution.h"

using namespace std;


CreatureNursery::CreatureNursery(): evolutionLibrary(new EvolutionLibrary()){
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


    Life * life = new Life("ANIMAL");
    life->setEntity(entity);
    life->setBrain(brain);

    EnergyManagement * energyManagement = new EnergyManagement();
    life->setEnergyManagement(energyManagement);
    energyManagement->setBody(entity);

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

    SelfEnergySensorEvolution * energySensorInputEvolution = new SelfEnergySensorEvolution();
    energySensorInputEvolution->setGenerationNumber(4);
    energySensorInputEvolution->perform(life);
    creatureGenome.emplace_back(energySensorInputEvolution);



    int sensorCount = rand() % 4 + 1;
    for (int it = 0; it < sensorCount; it++) {

        BarSensorDistanceEvolution * sensorEvol = new BarSensorDistanceEvolution();
        sensorEvol->setGenerationNumber((it * 3) + 5);
        sensorEvol->generateFromRandom(life);
        sensorEvol->perform(life);
        creatureGenome.emplace_back(sensorEvol);

    }

    SpeedEvolution * speedEvolution = new SpeedEvolution();
    speedEvolution->setGenerationNumber(35);
    speedEvolution->perform(life);
    creatureGenome.emplace_back(speedEvolution);

    RotationEvolution * rotationEvolution = new RotationEvolution();
    rotationEvolution->setGenerationNumber(36);
    rotationEvolution->perform(life);
    creatureGenome.emplace_back(rotationEvolution);

    MouthEvolution * mouthEvolution = new MouthEvolution();
    mouthEvolution->setGenerationNumber(37);
    mouthEvolution->generateFromRandom(life);
    mouthEvolution->perform(life);
    creatureGenome.emplace_back(mouthEvolution);

    GenitalsEvolution * genitalsEvolution = new GenitalsEvolution();
    genitalsEvolution->setGenerationNumber(38);
    genitalsEvolution->generateFromRandom(life);
    genitalsEvolution->perform(life);
    creatureGenome.emplace_back(genitalsEvolution);






    int generationNumberIndex(39);
    for (int it = 0; it < brain->getInputNeurons().size(); it++) {
        for (int jt = 0; jt < brain->getOutputNeurons().size(); jt++) {

            LinkEvolution * linkEvolution = new LinkEvolution();
            linkEvolution->setGenerationNumber(generationNumberIndex);
            linkEvolution->generateFromNeurons(life, brain->getInputNeurons().at(it), brain->getOutputNeurons().at(jt));
            linkEvolution->perform(life);
            generationNumberIndex++;
            creatureGenome.emplace_back(linkEvolution);

        }
    }


    brain->generateLinkGrid();

    life->connectSensorAndMuscles();

    evolutionLibrary->addGenome(life->getEntity()->getId(), creatureGenome);
    return life;
}


Life * CreatureNursery::Mate(Life * father, Life * mother) {

    std::vector<Evolution *> fatherGenome = this->getEvolutionLibrary()->getGenomeFor(father->getEntity()->getId());
    std::vector<Evolution *> motherGenome;

    if (mother != nullptr) {
        motherGenome = this->getEvolutionLibrary()->getGenomeFor(mother->getEntity()->getId());
    }

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

    float delta = (((rand() % 5000) / 100.f) - 5.f) * (father->getEnergyManagement()->getMaxMass() / 1000.0);
    float randomRotation = (((rand() % 2000) / 1000.f) - 1.0) * M_PI;

    Point childSpawn = {
    father->getEntity()->getPosition().getX() + cos(randomRotation) * delta,
    father->getEntity()->getPosition().getY() + sin(randomRotation) * delta,
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

    Life * life = new Life(father->getType());
    life->setEntity(childEntity);
    life->setBrain(childBrain);

    EnergyManagement * energyManagement = new EnergyManagement();
    life->setEnergyManagement(energyManagement);
    energyManagement->setBody(childEntity);


    for (int it = 0; it < childGenome.size(); it++) {
        childGenome.at(it)->perform(life);
    }

    life->getBrain()->generateLinkGrid();
    evolutionLibrary->addGenome(life->getEntity()->getId(), childGenome);

    life->connectSensorAndMuscles();
    return life;
}



Life * CreatureNursery::generateVegetalFromRandom() {
    random_device rd;
    mt19937 mt(rd());
    uniform_real_distribution<double> distWidth(0, FARM_WIDTH);
    uniform_real_distribution<double> distHeight(0, FARM_HEIGHT);

    int x = distWidth(mt);
    int y = distHeight(mt);

    Entity * entity = new Entity(Point(x, y));
    entity->setBrightness(0.5f);
    Brain * brain = new Brain();

    Life * life = new Life("VEGETAL");
    life->setEntity(entity);
    life->setBrain(brain);

    EnergyManagement * energyManagement = new EnergyManagement();
    life->setEnergyManagement(energyManagement);
    energyManagement->setBody(entity);

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

    SelfEnergySensorEvolution * energySensorInputEvolution = new SelfEnergySensorEvolution();
    energySensorInputEvolution->setGenerationNumber(4);
    energySensorInputEvolution->perform(life);
    creatureGenome.emplace_back(energySensorInputEvolution);

    AccessibleGroundEnergyEvolution * accessibleGroundEnergyEvolution = new AccessibleGroundEnergyEvolution();
    accessibleGroundEnergyEvolution->setGenerationNumber(5);
    accessibleGroundEnergyEvolution->perform(life);
    creatureGenome.emplace_back(accessibleGroundEnergyEvolution);

    TileHeatEvolution * tileHeatEvolution = new TileHeatEvolution();
    tileHeatEvolution->setGenerationNumber(6);
    tileHeatEvolution->perform(life);
    creatureGenome.emplace_back(tileHeatEvolution);


    DuplicationMuscleEvolution * duplicationMuscleEvolution = new DuplicationMuscleEvolution();
    duplicationMuscleEvolution->setGenerationNumber(7);
    duplicationMuscleEvolution->generateFromRandom(life);
    duplicationMuscleEvolution->perform(life);
    creatureGenome.emplace_back(duplicationMuscleEvolution);

//    CaptureHeatMuscleEvolution * captureHeatMuscleEvolution = new CaptureHeatMuscleEvolution();
//    captureHeatMuscleEvolution->setGenerationNumber(8);
//    captureHeatMuscleEvolution->perform(life);
//    creatureGenome.emplace_back(captureHeatMuscleEvolution);

    CaptureGroundMuscleEvolution * captureGroundMuscleEvolution = new CaptureGroundMuscleEvolution();
    captureGroundMuscleEvolution->setGenerationNumber(9);
    captureGroundMuscleEvolution->generateFromRandom(life);
    captureGroundMuscleEvolution->perform(life);
    creatureGenome.emplace_back(captureGroundMuscleEvolution);






//
//    RotationEvolution * rotationEvolution = new RotationEvolution();
//    rotationEvolution->setGenerationNumber(8);
//    rotationEvolution->perform(life);
//    creatureGenome.emplace_back(rotationEvolution);



    int generationNumberIndex(10);
    for (int it = 0; it < brain->getInputNeurons().size(); it++) {
        for (int jt = 0; jt < brain->getOutputNeurons().size(); jt++) {

            LinkEvolution * linkEvolution = new LinkEvolution();
            linkEvolution->setGenerationNumber(generationNumberIndex);
            linkEvolution->generateFromNeurons(life, brain->getInputNeurons().at(it), brain->getOutputNeurons().at(jt));
            linkEvolution->perform(life);
            generationNumberIndex++;
            creatureGenome.emplace_back(linkEvolution);

        }
    }

//
//    for (int it = 0; it < creatureGenome.size(); it++) {
//        std::cout << creatureGenome.at(it)->describe() << std::endl;
//    }
//    std::cout << std::endl;
//    std::cout << std::endl;


    brain->generateLinkGrid();

    life->connectSensorAndMuscles();

    evolutionLibrary->addGenome(life->getEntity()->getId(), creatureGenome);
    return life;
}


void CreatureNursery::setEvolutionLibrary(EvolutionLibrary *evolutionLibrary) {
    CreatureNursery::evolutionLibrary = evolutionLibrary;
}

EvolutionLibrary *CreatureNursery::getEvolutionLibrary() const {
    return evolutionLibrary;
}
