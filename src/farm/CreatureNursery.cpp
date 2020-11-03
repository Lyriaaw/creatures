//
// Created by Amalric Lombard de Buffières on 8/2/20.
//

#include <iostream>
#include "CreatureNursery.h"
#include "brain/BrainConnector.h"
#include "brain/neurons/BiasNeuron.h"
#include "evolutions/musclesEvolutions/SpeedEvolution.h"
#include "evolutions/nervous/DistanceBarSensorEvolution.h"
#include "evolutions/inputs/sensors/MassSensorEvolution.h"
#include "evolutions/musclesEvolutions/MemoryEvolution.h"
#include "evolutions/musclesEvolutions/PheromoneEmitterEvolution.h"
#include "evolutions/nervous/NeuronEvolution.h"
#include "evolutions/inputs/sensors/PheromoneSensorEvolution.h"

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
    entity->setIsLife(true);
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
    evolutionLibrary->addEvolution(colorEvolution);

    SizeEvolution * sizeEvolution = new SizeEvolution();
    sizeEvolution->setGenerationNumber(2);
    sizeEvolution->generateFromRandom(life);
    sizeEvolution->perform(life);
    creatureGenome.emplace_back(sizeEvolution);
    evolutionLibrary->addEvolution(sizeEvolution);



    BiasInputEvolution * biasInputEvolution = new BiasInputEvolution();
    biasInputEvolution->setGenerationNumber(3);
    biasInputEvolution->perform(life);
    creatureGenome.emplace_back(biasInputEvolution);
    evolutionLibrary->addEvolution(biasInputEvolution);

    EnergySensorEvolution * energySensorInputEvolution = new EnergySensorEvolution();
    energySensorInputEvolution->setGenerationNumber(4);
    energySensorInputEvolution->perform(life);
    creatureGenome.emplace_back(energySensorInputEvolution);
    evolutionLibrary->addEvolution(energySensorInputEvolution);

    MassSensorEvolution * massSensorEvolution = new MassSensorEvolution();
    massSensorEvolution->setGenerationNumber(5);
    massSensorEvolution->perform(life);
    creatureGenome.emplace_back(massSensorEvolution);
    evolutionLibrary->addEvolution(massSensorEvolution);




    double distanceRotations[] = {0.0, -0.25, 0.25, 1.0};

    for (int it = 0; it < 4; it++) {

        DistanceBarSensorEvolution * sensorEvol = new DistanceBarSensorEvolution();
        sensorEvol->setGenerationNumber(it + 10);
        sensorEvol->generateFromRandom(life);
        sensorEvol->setSensorLength(5);
        sensorEvol->setSensorRotation(distanceRotations[it]);
        sensorEvol->perform(life);
        creatureGenome.emplace_back(sensorEvol);
        evolutionLibrary->addEvolution(sensorEvol);

    }

    double colorRotations[] = {0.1, -0.1, 1.0};

    for (int it = 0; it < 3; it++) {
        HueBarSensorEvolution * sensorEvol = new HueBarSensorEvolution();
        sensorEvol->setGenerationNumber(it + 20);
        sensorEvol->generateFromRandom(life);
        sensorEvol->setSensorLength(4);
        sensorEvol->setSensorRotation(colorRotations[it]);

        sensorEvol->perform(life);
        creatureGenome.emplace_back(sensorEvol);
        evolutionLibrary->addEvolution(sensorEvol);
    }



    PheromoneSensorEvolution * pheromoneSensorEvolution = new PheromoneSensorEvolution();
    pheromoneSensorEvolution->setGenerationNumber(30);
    pheromoneSensorEvolution->generateFromRandom(life);
    pheromoneSensorEvolution->perform(life);
    creatureGenome.emplace_back(pheromoneSensorEvolution);
    evolutionLibrary->addEvolution(pheromoneSensorEvolution);

    PheromoneSensorEvolution * pheromoneSensorEvolution2 = new PheromoneSensorEvolution();
    pheromoneSensorEvolution2->setGenerationNumber(31);
    pheromoneSensorEvolution2->generateFromRandom(life);
    pheromoneSensorEvolution2->perform(life);
    creatureGenome.emplace_back(pheromoneSensorEvolution2);
    evolutionLibrary->addEvolution(pheromoneSensorEvolution2);
//
//    PheromoneSensorEvolution * pheromoneSensorEvolution3 = new PheromoneSensorEvolution();
//    pheromoneSensorEvolution3->setGenerationNumber(32);
//    pheromoneSensorEvolution3->generateFromRandom(life);
//    pheromoneSensorEvolution3->perform(life);
//    creatureGenome.emplace_back(pheromoneSensorEvolution3);
//    evolutionLibrary->addEvolution(pheromoneSensorEvolution3);






    SpeedEvolution * speedEvolution = new SpeedEvolution();
    speedEvolution->setGenerationNumber(40);
    speedEvolution->perform(life);
    creatureGenome.emplace_back(speedEvolution);
    evolutionLibrary->addEvolution(speedEvolution);

    RotationEvolution * rotationEvolution = new RotationEvolution();
    rotationEvolution->setGenerationNumber(41);
    rotationEvolution->perform(life);
    creatureGenome.emplace_back(rotationEvolution);
    evolutionLibrary->addEvolution(rotationEvolution);

    MouthEvolution * mouthEvolution = new MouthEvolution();
    mouthEvolution->setGenerationNumber(42);
    mouthEvolution->generateFromRandom(life);
    mouthEvolution->perform(life);
    creatureGenome.emplace_back(mouthEvolution);
    evolutionLibrary->addEvolution(mouthEvolution);

    GenitalsEvolution * genitalsEvolution = new GenitalsEvolution();
    genitalsEvolution->setGenerationNumber(43);
    genitalsEvolution->generateFromRandom(life);
    genitalsEvolution->perform(life);
    creatureGenome.emplace_back(genitalsEvolution);
    evolutionLibrary->addEvolution(genitalsEvolution);


    PheromoneEmitterEvolution * pheromoneEmitterEvolution = new PheromoneEmitterEvolution();
    pheromoneEmitterEvolution->setGenerationNumber(45);
    pheromoneEmitterEvolution->generateFromRandom(life);
    pheromoneEmitterEvolution->perform(life);
    creatureGenome.emplace_back(pheromoneEmitterEvolution);
    evolutionLibrary->addEvolution(pheromoneEmitterEvolution);

    PheromoneEmitterEvolution * pheromoneEmitterEvolution2 = new PheromoneEmitterEvolution();
    pheromoneEmitterEvolution2->setGenerationNumber(46);
    pheromoneEmitterEvolution2->generateFromRandom(life);
    pheromoneEmitterEvolution2->perform(life);
    creatureGenome.emplace_back(pheromoneEmitterEvolution2);
    evolutionLibrary->addEvolution(pheromoneEmitterEvolution2);

    MemoryEvolution * memoryEvolution = new MemoryEvolution();
    memoryEvolution->setGenerationNumber(50);
    memoryEvolution->generateFromRandom(life);
    memoryEvolution->perform(life);
    creatureGenome.emplace_back(memoryEvolution);
    evolutionLibrary->addEvolution(memoryEvolution);






    int generationNumberIndex(100);
    for (int it = 0; it < brain->getInputNeurons().size(); it++) {
        for (int jt = 0; jt < brain->getOutputNeurons().size(); jt++) {

            LinkEvolution * linkEvolution = new LinkEvolution();
            linkEvolution->setGenerationNumber(generationNumberIndex + (jt * 100) + it);
            linkEvolution->generateFromNeurons(life, brain->getInputNeurons().at(it), brain->getOutputNeurons().at(jt));
            linkEvolution->perform(life);
            creatureGenome.emplace_back(linkEvolution);
            evolutionLibrary->addEvolution(linkEvolution);

        }
    }




    brain->generateLinkGrid();

    life->connectSensorAndMuscles();

    evolutionLibrary->addGenome(life->getEntity()->getId(), creatureGenome);

    evolutionLibrary->setCurrentEvolutionNumber(1000);
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

    float deltaX = (((rand() % 400) / 100.f) - 2.f) * father->getEntity()->getSize();
    float deltaY = (((rand() % 400) / 100.f) - 2.f) * father->getEntity()->getSize();

    Point childSpawn = {
    father->getEntity()->getPosition().getX() + deltaX,
    father->getEntity()->getPosition().getY() + deltaY,
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
    childEntity->setIsLife(true);

    Brain * childBrain = new Brain();

    EnergyCenter * energyCenter = new EnergyCenter(childEntity);

    Life * life = new Life();
    life->setEntity(childEntity);
    life->setBrain(childBrain);
    life->setEnergyCenter(energyCenter);


    for (int it = 0; it < childGenome.size(); it++) {
        childGenome.at(it)->perform(life);
    }


    std::vector<Evolution *> randomEvolutions = generateNewRandomEvolution(life, childGenome);
    if (!randomEvolutions.empty()) {
        childGenome.insert(childGenome.end(), randomEvolutions.begin(), randomEvolutions.end());
    }

    life->getBrain()->generateLinkGrid();
    evolutionLibrary->addGenome(life->getEntity()->getId(), childGenome);

    life->connectSensorAndMuscles();
    return life;
}

std::vector<Evolution *> CreatureNursery::generateNewRandomEvolution(Life * life, std::vector<Evolution *> childGenome) {
    std::vector<Evolution *> newEvolutions;

    if (rand() % MUTATION_RATIO != 0) {
        return newEvolutions;
    }

    std::cout << "New evolution " << std::endl;

    return generateNeuronEvolution(life, childGenome);
}

std::vector<Evolution *> CreatureNursery::generateNeuronEvolution(Life * life, std::vector<Evolution *> childGenome) {
    std::vector<Evolution *> newEvolutions;

    int neuronsSize = life->getBrain()->getNeurons().size();

    Neuron * firstNeuron = life->getBrain()->getNeurons().at(rand() % neuronsSize);
    Neuron * secondNeuron = nullptr;
    bool correctPick = false;

    while(!correctPick) {
        secondNeuron = life->getBrain()->getNeurons().at(rand() % neuronsSize);

        correctPick = firstNeuron->getGenerationNumber() != secondNeuron->getGenerationNumber() && firstNeuron->getX() != secondNeuron->getX();
    }

    Neuron * leftNeuron = nullptr;
    Neuron * rightNeuron = nullptr;
    if (firstNeuron->getX() > secondNeuron->getX()) {
        rightNeuron = firstNeuron;
        leftNeuron = secondNeuron;
    } else {
        rightNeuron = secondNeuron;
        leftNeuron = firstNeuron;
    }


    LinkEvolution * foundEvolution = nullptr;

    for (int it = 0; it < childGenome.size(); it++) {
        if (childGenome.at(it)->getName() != "Link") {
            continue;
        }

        LinkEvolution * currentEvolution = (LinkEvolution *) childGenome.at(it);

        bool sameInput = currentEvolution->getInputGenerationNumber() == leftNeuron->getGenerationNumber();
        bool sameOutput = currentEvolution->getOutputGenerationNumber() == rightNeuron->getGenerationNumber();

        if (sameInput && sameOutput) {
            foundEvolution = currentEvolution;
        }

    }

    if (foundEvolution) {
        if (foundEvolution->isEnabled()) {
            return generateNeuronEvolutionFromLinkEvolution(leftNeuron, rightNeuron, foundEvolution, life);
        } else {
            evolutionLibrary->recordEvent(std::to_string(leftNeuron->getGenerationNumber()) + " => " + std::to_string(rightNeuron->getGenerationNumber()) + " : Already a disabled evolution here");
            return newEvolutions;
        }

    } else {
        evolutionLibrary->recordEvent(std::to_string(leftNeuron->getGenerationNumber()) + " => " + std::to_string(rightNeuron->getGenerationNumber()) + " : New link evolution");
        return generateLinkEvolution(leftNeuron, rightNeuron, life);
    }

    return newEvolutions;
}

std::vector<Evolution *> CreatureNursery::generateLinkEvolution(Neuron * inputNeuron, Neuron * outputNeuron, Life * life) {
    std::vector<Evolution *> newEvolutions;

    LinkEvolution * linkEvolution = new LinkEvolution();
    linkEvolution->generateGenerationNumber();
    linkEvolution->generateFromNeurons(life, inputNeuron, outputNeuron);

    std::map<int, Evolution *> allEvolutions = evolutionLibrary->getAllEvolutions();

    bool found = false;
    for (auto const& row: allEvolutions) {
        if (row.second->getName() != "Link") {
            continue;
        }

        LinkEvolution * currentExistingLinkEvolution = (LinkEvolution *) row.second;

        if (currentExistingLinkEvolution->getInputGenerationNumber() == linkEvolution->getInputGenerationNumber()
            && currentExistingLinkEvolution->getOutputGenerationNumber() == linkEvolution->getOutputGenerationNumber()) {
            found = true;
            linkEvolution->setGenerationNumber(currentExistingLinkEvolution->getGenerationNumber());
        }
    }

    linkEvolution->perform(life);
    newEvolutions.emplace_back(linkEvolution);

    if (!found) {
        evolutionLibrary->recordEvent("  - Created new evolution");
        evolutionLibrary->addEvolution(linkEvolution);
    } else {
        evolutionLibrary->recordEvent("  - Link evolution already exists in the library => " + std::to_string(linkEvolution->getGenerationNumber()));
    }

    return newEvolutions;

}


std::vector<Evolution *> CreatureNursery::generateNeuronEvolutionFromLinkEvolution(Neuron * inputNeuron, Neuron * outputNeuron, LinkEvolution * linkEvolution, Life * life) {
    std::vector<Evolution *> newEvolutions;

    float neuronX = (inputNeuron->getX() + outputNeuron->getX()) / 2.f;
    float neuronY = (inputNeuron->getY() + outputNeuron->getY()) / 2.f;





    NeuronEvolution * neuronEvolution = new NeuronEvolution();
    neuronEvolution->generateGenerationNumber();
    neuronEvolution->generateFromXandY(neuronX, neuronY);

    evolutionLibrary->recordEvent(std::to_string(inputNeuron->getGenerationNumber()) + " => " + std::to_string(outputNeuron->getGenerationNumber()) + " : New neuron");
    evolutionLibrary->recordEvent("  - Initial generation number: " + std::to_string(neuronEvolution->getGenerationNumber()));
    evolutionLibrary->recordEvent("  - X: " + std::to_string(neuronX) + " Y: " + std::to_string(neuronY));

    Neuron * selectedNeuron = nullptr;
    bool found = false;
    for (int it = 0; it < life->getBrain()->getNeurons().size(); it++) {
        Neuron * currentNeuron = life->getBrain()->getNeurons().at(it);

        float absX = abs(currentNeuron->getX() - neuronEvolution->getX());
        float absY = abs(currentNeuron->getY() - neuronEvolution->getY());

        if (absX < 10 && absY < 10) {
            selectedNeuron = currentNeuron;
            found = true;
            evolutionLibrary->recordEvent("  - Found another neuron in the brain already: " + std::to_string(currentNeuron->getGenerationNumber()) + " (" + std::to_string(neuronX) + " , " + std::to_string(neuronY) + ")");
        }
    }

    bool isNew = true;
    if (!found) {
        std::map<int, Evolution *> allEvolutions = evolutionLibrary->getAllEvolutions();

        for (auto const& currentEvolution : allEvolutions) {

            if (currentEvolution.second->getName() != "Neuron") {
                continue;
            }

            NeuronEvolution * currentNeuronEvolution = (NeuronEvolution *) currentEvolution.second;

            float absX = abs(currentNeuronEvolution->getX() - neuronEvolution->getX());
            float absY = abs(currentNeuronEvolution->getY() - neuronEvolution->getY());

            if (absX < 10 && absY < 10) {
                neuronEvolution->setGenerationNumber(currentNeuronEvolution->getGenerationNumber());
                neuronEvolution->generateFromXandY(currentNeuronEvolution->getX(), currentNeuronEvolution->getY());
                isNew = false;
                evolutionLibrary->recordEvent("  - FFound another at the same place in the library: " + std::to_string(currentNeuronEvolution->getGenerationNumber()) + " (" + std::to_string(neuronEvolution->getX()) + " , " + std::to_string(neuronEvolution->getY()) + ")");
            }
        }
    }



    if (!found) {
        neuronEvolution->perform(life);
        newEvolutions.emplace_back(neuronEvolution);
        if (isNew) {
            evolutionLibrary->recordEvent("  - No neuron found already. Will be added in the evolution library");
            evolutionLibrary->addEvolution(neuronEvolution);
        }
        selectedNeuron = neuronEvolution->getNeuron();
    }


    if (selectedNeuron == nullptr) {
        std::cout << "Error while selecting new neuron for link" << std::endl;
        return newEvolutions;
    }


    std::vector<Evolution *> outputLinkEvolutions = generateLinkEvolution(selectedNeuron, outputNeuron, life);
    newEvolutions.insert(newEvolutions.end(), outputLinkEvolutions.begin(), outputLinkEvolutions.end());
    evolutionLibrary->recordEvent("  - Connected to right neuron: " + std::to_string(outputNeuron->getGenerationNumber()));



    std::vector<Evolution *> inputLinkEvolutions = generateLinkEvolution(inputNeuron, selectedNeuron, life);
    newEvolutions.insert(newEvolutions.end(), inputLinkEvolutions.begin(), inputLinkEvolutions.end());
    evolutionLibrary->recordEvent("  - Connected to right neuron: " + std::to_string(inputNeuron->getGenerationNumber()));


    evolutionLibrary->recordEvent("  - Connected to left neuron: " + std::to_string(inputNeuron->getGenerationNumber()));

    life->getBrain()->removeLink(inputNeuron->getGenerationNumber(), outputNeuron->getGenerationNumber());
    linkEvolution->disable();
    evolutionLibrary->recordEvent("  - Removed link between and disabling evolution between " + std::to_string(inputNeuron->getGenerationNumber()) + " and " + std::to_string(outputNeuron->getGenerationNumber()));


    return newEvolutions;
}


const EvolutionLibrary &CreatureNursery::getEvolutionLibrary() const {
    return *evolutionLibrary;
}


//HueBarSensorEvolution * sensorEvol = new HueBarSensorEvolution();
//sensorEvol->generateGenerationNumber();
//sensorEvol->generateFromRandom(life);
//sensorEvol->perform(life);
//newEvolutions.emplace_back(sensorEvol);
//evolutionLibrary->addEvolution(sensorEvol);
//
//for (int jt = 0; jt < life->getBrain()->getOutputNeurons().size(); jt++) {
//
//LinkEvolution * linkEvolution = new LinkEvolution();
//linkEvolution->generateGenerationNumber();
//linkEvolution->generateFromNeurons(life, sensorEvol->getInputNeuron(), life->getBrain()->getOutputNeurons().at(jt));
//linkEvolution->perform(life);
//newEvolutions.emplace_back(linkEvolution);
//evolutionLibrary->addEvolution(linkEvolution);

//}