//
// Created by Amalric Lombard de Buffières on 7/17/20.
//

#include "Farm.h"
#include "perlin/PerlinNoise.h"

#include <iostream>
#include <sstream>
#include <iomanip>


using namespace std;

Farm::Farm() {
}

void Farm::InitRandomMap() {
    float coolSeeds[] = {3041, 7980, 4672, 2354, 518, 6237, 868, 3815, 2727, 1568, 5953, 8058, 568654, 787145, 924505, 117802, 523117, 45482, 407575, 391032, 660340, 526115, 218205, 890683, 595048, 236189, 781676, 260093, 693209, 80826, 345867, 758436, 439334 };
    float seed = rand() % 1000000;
//    float seed = 1568;
    PerlinNoise perlin(seed);

    cout << "Map generated with seed " << seed << endl;

    map = Map();
    for (int it = 0; it < CHUNK_COUNT_WIDTH; it++) {
        for (int jt = 0; jt < CHUNK_COUNT_HEIGHT; jt++) {
            float height = perlin.noise(float(it) / float(CHUNK_COUNT_WIDTH) * 2.5, float(jt) / float(CHUNK_COUNT_HEIGHT) * 2.5, 0.8) - 0.4f;


            map.setTileAt(it, jt, height);
        }
    }

    for (int it = 0; it < CHUNK_COUNT_WIDTH; it++) {
        std::vector<std::vector<Entity *>> line;
        for (int jt = 0; jt < CHUNK_COUNT_HEIGHT; jt++) {
            std::vector<Entity *> currentChunk;
            line.push_back(currentChunk);
        }
        entityGrid.push_back(line);
    }
}

void Farm::InitFromRandom() {
    random_device rd;
    mt19937 mt(rd());
    uniform_real_distribution<double> distWidth(0, FARM_WIDTH);
    uniform_real_distribution<double> distHeight(0, FARM_HEIGHT);

    InitRandomMap();

    std::uniform_real_distribution<float> distMovement(-1, 1);
    nursery = new CreatureNursery();
    for (int it = 0; it < 100; it++) {
        BrainConnector * initialCreature = nursery->generateFromRandom();

        float creatureEnergy = initialCreature->getCreature()->getMaxEnergy() / 2.0;
        initialCreature->getCreature()->setEnergy(creatureEnergy);

        connectors.push_back(initialCreature);
    }

    for (int it = 0; it < 50000; it++) {
        int x = distWidth(mt);
        int y = distHeight(mt);

        Point point(x, y);


//        float foodSize = ((rand() % 300) / 100.f) + 2;
        float foodSize = 2;

        Food * entity = new Food(point, foodSize);
        entity->setEnergy(entity->getMaxEnergy());
        foods.push_back(entity);
    }

    availableEnergy = 0.f;
    tickCount = 0;
}


void Farm::Tick(bool paused) {
    random_device rd;
    mt19937 mt(rd());
    uniform_real_distribution<double> distWidth(0, FARM_WIDTH);
    uniform_real_distribution<double> distHeight(0, FARM_HEIGHT);


    averageSelectedEntities = 0;

    this->toDelete.clear();

    if (!paused) {

        std::vector<BrainConnector *> sortedConnectors = getScoreSortedCreatures();


        for (int it = 0; it < sortedConnectors.size(); it++) {
            sortedConnectors.at(it)->processBrainOutputs();
        }

        executeCreaturesActions();

        for (int it = 0; it < sortedConnectors.size(); it++) {

            Creature *currentCreature = sortedConnectors.at(it)->getCreature();
            float givenEnergy =currentCreature->move();
            availableEnergy += givenEnergy;

            if (givenEnergy < 0) {
                std::cout << "Returned negative amount of energy: " << givenEnergy << std::endl;

            }

            if (currentCreature->getEnergy() <= 0) {
                if (currentCreature->getEnergy() < 0) {
                    std::cout << "Creature with less than 0 energy: " << currentCreature->getEnergy() << std::endl;
                }
                toDelete.emplace_back(currentCreature);
            }
        }
    }

    clearDeletedEntities();

    if (this->connectors.size() < 500) {
        std::vector<BrainConnector *> sortedConnectors = getScoreSortedCreatures();
        int selectedParentCount = sortedConnectors.size() / 2;

        int newConnectorNeeded = 55 - this->connectors.size();
//        std::cout << newConnectorNeeded << " new creatures needed" << std::endl;

        for (int it = 0; it < newConnectorNeeded; it++) {
            int fatherIndex = rand() % selectedParentCount;
            int motherIndex = rand() % selectedParentCount;

            BrainConnector * father = sortedConnectors.at(fatherIndex);
            BrainConnector * mother = sortedConnectors.at(motherIndex);

            BrainConnector * child = this->nursery->Mate(father, mother);
            child->getCreature()->setEnergy(child->getCreature()->getMaxEnergy() / 4.f);
            Creature * childCreature = child->getCreature();

            float childSpawnX = distWidth(mt);
            float childSpawnY = distHeight(mt);
            Point childCreaturePosition = Point(childSpawnX, childSpawnY);

            childCreature->setPosition(childCreaturePosition);

            float energyToRemove = child->getCreature()->getEnergy();

            if (availableEnergy >= energyToRemove) {
                availableEnergy -= energyToRemove;
            } else {
                float totalCollected = 0.f;
                for (int jt = 0; jt < foods.size(); jt++) {
                    if (totalCollected >= energyToRemove)
                        continue;

                    if (foods.at(jt)->getEnergy() <= 0) {
                        continue;
                    }

                    totalCollected += foods.at(jt)->removeEnergy(foods.at(jt)->getEnergy());

                    toDelete.emplace_back(foods.at(jt));
                }

                if (totalCollected > energyToRemove) {
                    availableEnergy += totalCollected - energyToRemove;
                }

            }


            connectors.emplace_back(child);
            addedCreatures.emplace_back(child->getCreature());


        }
    }
    clearDeletedEntities();


    int foodToGenerate = (int(availableEnergy) / 2000) - 1;

    float totalEnergyAdded = 0.f;



    for (int it = 0; it < foodToGenerate; it++) {
        int x = distWidth(mt);
        int y = distHeight(mt);

        Point point(x, y);


//        float foodSize = ((rand() % 300) / 100.f) + 2;
        float foodSize = 2;

        Food * entity = new Food(point, foodSize);
        entity->setEnergy(entity->getMaxEnergy());

        availableEnergy -= entity->getEnergy();
        totalEnergyAdded += entity->getEnergy();


        foods.emplace_back(entity);
        addedEntity.emplace_back(entity);
    }






    if (tickCount % 100 == 0) {
//    if (true) {
        double totalCreaturesEnergy = 0.f;
        double totalFoodsEnergy = 0.f;

        for (int it = 0; it < connectors.size(); it++) {
            Creature * currentCreature = connectors.at(it)->getCreature();
            totalCreaturesEnergy += currentCreature->getEnergy();
        }

        for (int it = 0; it < foods.size(); it++) {
            Entity * entity = foods.at(it);
            totalFoodsEnergy += entity->getEnergy();
        }


        std::vector<BrainConnector *> sortedConnectors = getScoreSortedCreatures();




        std::cout << "Creatures: " << connectors.size() << " Entities: " << foods.size();

        int totalEnergy = availableEnergy + totalFoodsEnergy + totalCreaturesEnergy;
        std::cout << "  | Tick: " << tickCount << " total: " << getHumanReadableEnergy(totalEnergy);
        std::cout << "  --  available: " << getHumanReadableEnergy(availableEnergy);
        std::cout << "  --  creatures: " << getHumanReadableEnergy(totalCreaturesEnergy);
        std::cout << "  --  foods: " << getHumanReadableEnergy(totalFoodsEnergy);
        std::cout << "  --  raw total: " << totalEnergy;
        std::cout << std::endl;


    }



    generateEntityGrid();

    if (!paused) {
        for (int it = 0; it < connectors.size(); it++) {
            Creature *currentCreature = connectors.at(it)->getCreature();

            std::vector<Entity *> selectedEntities = getAccessibleEntities(currentCreature);

            std::vector<ActionDTO> currentCreatureActions = currentCreature->executeAction(selectedEntities);
            actions.insert(actions.end(), currentCreatureActions.begin(), currentCreatureActions.end());

            averageSelectedEntities += selectedEntities.size();
        }
    }


    for (int it = 0; it < connectors.size(); it++) {
        Creature * currentCreature = connectors.at(it)->getCreature();

        currentCreature->findSelectedChunks();
        currentCreature->processSensorsValues(getAccessibleEntities(currentCreature));
        connectors.at(it)->processBrainInputs();
        connectors.at(it)->processBrain();
    }


    for (int it = 0; it < connectors.size(); it++) {
        connectors.at(it)->getCreature()->aTickHavePassed();
    }
    for (int it = 0; it < foods.size(); it++) {
        foods.at(it)->aTickHavePassed();
    }




    averageSelectedEntities /= float(connectors.size());


    tickCount++;
}


void Farm::executeCreaturesActions() {

    for (int it = 0; it < actions.size(); it++) {
        ActionDTO currentAction = actions.at(it);
        BrainConnector * performer = getConnectorFromId(currentAction.getPerformerId());
        Entity * subject = getEntityFromId(currentAction.getSubjectId());

        bool found = false;
        for (int jt = 0; jt < toDelete.size(); jt++) {
            if (subject->getId() == toDelete.at(jt)->getId()) {
//                std::cout << "Performer is about to be delete" << std::endl;
                found = true;
            }

            if (performer->getCreature()->getId() == toDelete.at(jt)->getId()) {
                std::cout << "Subject is about to be delete" << std::endl;
                found = true;
            }
        }
        if (found)
            continue;


        if (performer == nullptr || subject == nullptr) {
            std::cout << "Unable to perform action " << currentAction.getType() << "(" << currentAction.getPerformerId() << " -> " << currentAction.getSubjectId() << ")" << std::endl;
            continue;
        }

        if (currentAction.getType() == "EAT") {
            availableEnergy += performer->getCreature()->addEnergy(subject->getEnergy());
            toDelete.emplace_back(subject);
        }

        if (currentAction.getType() == "MATE") {
            // TODO Verify that the partner is trying to reproduce as well (let's implement consentment)
            BrainConnector * foundConnector = getConnectorFromId(subject->getId());
            if (foundConnector == nullptr) {
                continue;
            }

            bool fatherCanReproduce = performer->getCreature()->getEnergy() > performer->getCreature()->getMaxEnergy() / 2.f;
            bool motherCanReproduce = foundConnector->getCreature()->getEnergy() > performer->getCreature()->getMaxEnergy() / 2.f;

            if (!fatherCanReproduce || !motherCanReproduce) {
                continue;
            }

//            std::cout << "Mating " << std::endl;
//
            BrainConnector * child = this->nursery->Mate(performer, foundConnector);

            float givenEnergyToChildGoal = child->getCreature()->getMaxEnergy() / 4.f;

            float givenFatherEnergy = performer->getCreature()->removeEnergy(givenEnergyToChildGoal / 2.f);
            float givenMotherEnergy = foundConnector->getCreature()->removeEnergy(givenEnergyToChildGoal / 2.f);

            child->getCreature()->setEnergy(givenFatherEnergy + givenMotherEnergy);
            connectors.emplace_back(child);

            addedCreatures.emplace_back(child->getCreature());
        }


    }


    actions.clear();
}


void Farm::generateEntityGrid() {
    for (int it = 0; it < CHUNK_COUNT_WIDTH; it++) {
        for (int jt = 0; jt < CHUNK_COUNT_HEIGHT; jt++) {
            entityGrid.at(it).at(jt).clear();
        }
    }

    for (int it = 0; it < connectors.size(); it++) {
        Point simpleCoordinates = connectors.at(it)->getCreature()->getSimpleCoordinates();
        entityGrid.at(simpleCoordinates.getX()).at(simpleCoordinates.getY()).push_back(connectors.at(it)->getCreature());
    }

    for (int it = 0; it < foods.size(); it++) {
        Point simpleCoordinates = foods.at(it)->getSimpleCoordinates();
        entityGrid.at(simpleCoordinates.getX()).at(simpleCoordinates.getY()).push_back(foods.at(it));
    }

}


void Farm::clearDeletedEntities() {

    std::vector<BrainConnector *> newConnectors;
    for (int it = 0; it < connectors.size(); it++) {

        bool found = false;
        for (int jt = 0; jt < toDelete.size(); jt++) {
            if (connectors.at(it)->getCreature()->getId() == toDelete.at(jt)->getId()) {
                found = true;
            }
        }

        if (!found) {
            newConnectors.emplace_back(connectors.at(it));
        }
    }

    connectors.clear();
    connectors = newConnectors;

    std::vector<Food *> newFood;
    for (int it = 0; it < foods.size(); it++) {

        bool found = false;
        for (int jt = 0; jt < toDelete.size(); jt++) {
            if (foods.at(it)->getId() == toDelete.at(jt)->getId()) {
                found = true;
            }
        }

        if (!found) {
            newFood.emplace_back(foods.at(it));
        }
    }

    foods.clear();
    foods = newFood;


    actions.clear();
}

Creature * Farm::getCreatureFromId(int id) {
    for (int it = 0; it < this->connectors.size(); it++) {
        if (this->connectors.at(it)->getCreature()->getId() == id) {
            return this->connectors.at(it)->getCreature();
        }
    }

    return nullptr;
}
BrainConnector * Farm::getConnectorFromId(int id) {
    for (int it = 0; it < this->connectors.size(); it++) {
        if (this->connectors.at(it)->getCreature()->getId() == id) {
            return this->connectors.at(it);
        }
    }

    return nullptr;
}
Entity * Farm::getEntityFromId(int id) {
    for (int it = 0; it < this->connectors.size(); it++) {
        if (this->connectors.at(it)->getCreature()->getId() == id) {
            return this->connectors.at(it)->getCreature();
        }
    }

    for (int it = 0; it < this->foods.size(); it++) {
        if (this->foods.at(it)->getId() == id) {
            return this->foods.at(it);
        }
    }


    return nullptr;
}

std::vector<Entity *> Farm::getAccessibleEntities(Creature * creature) {
    std::vector<Entity *> accessibleEntities;
    for (int jt = 0; jt < creature->getSelectedChunks().size(); jt++) {
        Point currentChunk = creature->getSelectedChunks().at(jt);

        std::vector<Entity *> chunkEntities = entityGrid.at(currentChunk.getX()).at(currentChunk.getY());
        accessibleEntities.insert(accessibleEntities.end(), chunkEntities.begin(), chunkEntities.end());
    }
    return accessibleEntities;

}





Map * Farm::getMap() {
    return &map;
}

const vector<Food *> &Farm::getFoods() const {
    return foods;
}

CreatureNursery *Farm::getNursery() const {
    return nursery;
}

const vector<BrainConnector *> &Farm::getConnectors() const {
    return connectors;
}

FarmUI *Farm::getUi() const {
    return ui;
}

void Farm::setUi(FarmUI *ui) {
    Farm::ui = ui;
}

void Farm::addConnector(BrainConnector * connector) {
    this->connectors.push_back(connector);
}

float Farm::getAverageSelectedEntities() const {
    return averageSelectedEntities;
}

const vector<Entity *> &Farm::getToDelete() const {
    return toDelete;
}

std::string Farm::getHumanReadableEnergy(float givenEnergy) {
    std::stringstream givenEnergyStream;


    if (givenEnergy > 1000000000.f) {
        givenEnergyStream << std::fixed << std::setprecision(2) << givenEnergy / 1000000000.f;
        givenEnergyStream << " GA";
        return givenEnergyStream.str();
    }

    if (givenEnergy > 1000000.f) {
        givenEnergyStream << std::fixed << std::setprecision(2) << givenEnergy / 1000000.f;
        givenEnergyStream << " mA";
        return givenEnergyStream.str();
    }

    if (givenEnergy > 1000.f) {
        givenEnergyStream << std::fixed << std::setprecision(2) << givenEnergy / 1000.f;
        givenEnergyStream << " kA";
        return givenEnergyStream.str();
    }


    givenEnergyStream << givenEnergy;
    givenEnergyStream << " A";







    return givenEnergyStream.str();
}

void Farm::clearAddedCreatures() {
    this->addedCreatures.clear();
}
void Farm::clearAddedEntities() {
    this->addedEntity.clear();
}

std::vector<BrainConnector *> Farm::getScoreSortedCreatures() {
    std::vector<BrainConnector *> sorted;

    std::vector<BrainConnector *> tmpConnectors = this->connectors;

    while (!tmpConnectors.empty()) {

        int biggestIndex = -1;
        float biggestScore = 0.f;

        for (int it = 0; it < tmpConnectors.size(); it++) {
            if (tmpConnectors.at(it)->getCreature()->getAge() >= biggestScore) {
                biggestIndex = it;
                biggestScore = tmpConnectors.at(it)->getCreature()->getAge();
            }
        }

        if (biggestIndex == -1) {
            std::cout << "Error while sorting creatures by score - Life will probably crash" << std::endl;
        }

        sorted.emplace_back(tmpConnectors.at(biggestIndex));
        tmpConnectors.erase(tmpConnectors.begin() + biggestIndex);
    }

    return sorted;
}



const vector<Food *> &Farm::getAddedEntity() const {
    return addedEntity;
}

void Farm::setAddedEntity(const vector<Food *> &addedEntity) {
    Farm::addedEntity = addedEntity;
}

const vector<Creature *> &Farm::getAddedCreatures() const {
    return addedCreatures;
}

void Farm::setAddedCreatures(const vector<Creature *> &addedCreatures) {
    Farm::addedCreatures = addedCreatures;
}

