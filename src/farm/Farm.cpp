//
// Created by Amalric Lombard de Buffières on 7/17/20.
//

#include "Farm.h"
#include "perlin/PerlinNoise.h"

#include <iostream>
#include <sstream>
#include <iomanip>


using namespace std;

Farm::Farm(){
    tickStart = std::chrono::system_clock::now();
    tickEnd = std::chrono::system_clock::now();
}

void Farm::InitRandomMap() {
    float seed = rand() % 1000000;
//    float seed = 1568;
    PerlinNoise perlin(seed);

    cout << "Map generated with seed " << seed << endl;
    cout << "Map Width: " << TILE_COUNT_WIDTH << " Map Height: " << TILE_COUNT_HEIGHT << endl;

    float min = 11111110.f;
    float max = 0.f;

    map = Map();
    for (int it = 0; it < TILE_COUNT_WIDTH; it++) {
        for (int jt = 0; jt < TILE_COUNT_HEIGHT; jt++) {

            float xComponent = (float(it) / float(TILE_COUNT_WIDTH)) * 2.5;
            float yComponent = (float(jt) / float(TILE_COUNT_HEIGHT)) * 2.5;

            float height = perlin.noise(xComponent, yComponent, 0.8);


//            std::cout << "X: " << it << " Y: " << jt << " Value: " << height << " -- Components: x: " << xComponent << " y: " << yComponent << std::endl;
            if (height < min) {
                min = height;
            }
            if (height > max) {
                max = height;
            }



            map.setTileAt(it, jt, height);
        }
    }
//    std::cout << "Min: " << min << " Max: " << max << " Removed:" << 0 << std::endl;



    float removed = ((max - min) / 2.f) + min;

    float ratio = (1.f / (max - min)) * 2.f;

    min = 11111110.f;
    max = 0.f;
    for (int it = 0; it < TILE_COUNT_WIDTH; it++) {
        for (int jt = 0; jt < TILE_COUNT_HEIGHT; jt++) {
            float currentHeight = map.getTileAt(it, jt);

            currentHeight -= removed;
            currentHeight *= ratio;
            currentHeight *= abs(currentHeight) * abs(currentHeight);


            if (currentHeight < min) {
                min = currentHeight;
            }
            if (currentHeight > max) {
                max = currentHeight;
            }



            map.setTileAt(it, jt,  currentHeight);
        }
    }


    std::cout << "Min: " << min << " Max: " << max << " Removed:" << 0 << std::endl;


    std::vector<std::vector<std::vector<Entity *>>> testEntites;

    for (int it = 0; it < CHUNK_COUNT_WIDTH; it++) {
        std::vector<std::vector<Entity *>> line;
        for (int jt = 0; jt < CHUNK_COUNT_HEIGHT; jt++) {
            std::vector<Entity *> currentChunk;
            line.push_back(currentChunk);
        }
        testEntites.emplace_back(line);
    }

    entityGrid = testEntites;
}

void Farm::InitFromRandom() {
    random_device rd;
    mt19937 mt(rd());
    uniform_real_distribution<double> distWidth(0, FARM_WIDTH);
    uniform_real_distribution<double> distHeight(0, FARM_HEIGHT);

    InitRandomMap();

    std::uniform_real_distribution<float> distMovement(-1, 1);
    nursery = new CreatureNursery();
    for (int it = 0; it < INITIAL_CREATURE_COUNT; it++) {
        BrainConnector * initialCreature = nursery->generateFromRandom();

        float creatureEnergy = initialCreature->getCreature()->getMaxEnergy() / 2.0;
        initialCreature->getCreature()->setEnergy(creatureEnergy);

        connectors.push_back(initialCreature);
    }

    for (int it = 0; it < INITIAL_FOOD_COUNT; it++) {
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
    this->toDelete.clear();

    generateEntityGrid();

    if (!paused) {
        brainOutput();
        moveCreatures();
        handleActions();
    }


    populationControl();
    vegetalisation();

    brainProcessing();

    if (!paused) {
        aTickHavePassed();
        statistics();
    }


    tickEnd = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_time = tickEnd - tickStart;
    tickStart = std::chrono::system_clock::now();

    if (!paused) {
        double tickTime = elapsed_time.count();
        if (tickCount == 1) {
            dataAnalyser.getTickTime()->addValue(0);
        } else {
            dataAnalyser.getTickTime()->addValue(tickTime);
        }

        dataAnalyser.getTickPerSecond()->addValue(1.0 / tickTime);
    }
}



void Farm::brainProcessing() {
    std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
    for (int it = 0; it < connectors.size(); it++) {
        Creature * currentCreature = connectors.at(it)->getCreature();

        currentCreature->findSelectedChunks();
        currentCreature->processSensorsValues(getAccessibleEntities(currentCreature));
        connectors.at(it)->processBrainInputs();
        connectors.at(it)->processBrain();
    }

    std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_time = end - start;
    dataAnalyser.getBrainProcessingTime()->addValue(elapsed_time.count());
}

void Farm::brainOutput() {
    std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
    for (int it = 0; it < connectors.size(); it++) {
        connectors.at(it)->processBrainOutputs();
    }
    std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_time = end - start;
    dataAnalyser.getBrainOutputsTime()->addValue(elapsed_time.count());
}

void Farm::moveCreatures() {
    std::chrono::system_clock::time_point start = std::chrono::system_clock::now();

    for (int it = 0; it < connectors.size(); it++) {
        Creature *currentCreature = connectors.at(it)->getCreature();
        double givenEnergy = currentCreature->move();
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

    clearDeletedEntities();

    std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_time = end - start;
    dataAnalyser.getMoveCreaturesTime()->addValue(elapsed_time.count());
}



void Farm::handleActions() {
    std::chrono::system_clock::time_point start = std::chrono::system_clock::now();

    std::vector<BrainConnector *> sortedConnectors = getScoreSortedCreatures();

    for (int it = 0; it < sortedConnectors.size(); it++) {
        Creature *currentCreature = sortedConnectors.at(it)->getCreature();

        std::vector<Entity *> selectedEntities = getAccessibleEntities(currentCreature);
        std::vector<ActionDTO> currentCreatureActions = currentCreature->executeAction(selectedEntities);

        actions.insert(actions.end(), currentCreatureActions.begin(), currentCreatureActions.end());
    }

    std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_time = end - start;
    dataAnalyser.getPrepareActionsTime()->addValue(elapsed_time.count());

    executeCreaturesActions();

}

void Farm::executeCreaturesActions() {
    std::chrono::system_clock::time_point start = std::chrono::system_clock::now();

    for (int it = 0; it < actions.size(); it++) {
        ActionDTO actionDto = actions.at(it);

        if (isEntityAboutToBeDeleted(actionDto.getPerformerId())) {
            continue;
        }

        if (isEntityAboutToBeDeleted(actionDto.getSubjectId())) {
            continue;
        }

        BrainConnector * performer = getConnectorFromId(actionDto.getPerformerId());
        Entity * subject = getEntityFromId(actionDto.getSubjectId());


        if (actionDto.getType() == "EAT") {
            availableEnergy += performer->getCreature()->addEnergy(subject->getEnergy());
            toDelete.emplace_back(subject);
        }

        if (actionDto.getType() == "MATE") {
            handleMating(performer, subject->getId());

        }

        clearDeletedEntities();
    }

    actions.clear();

    std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_time = end - start;
    dataAnalyser.getExecuteActionsTime()->addValue(elapsed_time.count());

}

void Farm::handleMating(BrainConnector * father, int entityId) {
// TODO Verify that the partner is trying to reproduce as well (let's implement consentment)
    BrainConnector * foundConnector = getConnectorFromId(entityId);
    if (foundConnector == nullptr) {
        return;
    }

    bool fatherCanReproduce = father->getCreature()->getEnergy() > father->getCreature()->getMaxEnergy() / 2.f;
    bool motherCanReproduce = foundConnector->getCreature()->getEnergy() > father->getCreature()->getMaxEnergy() / 2.f;

    if (!fatherCanReproduce || !motherCanReproduce) {
        return;
    }
//
    BrainConnector * child = this->nursery->Mate(father, foundConnector);

    double givenEnergyToChildGoal = child->getCreature()->getMaxEnergy() / 4.f;

    double givenFatherEnergy = father->getCreature()->removeEnergy(givenEnergyToChildGoal / 2.f);
    double givenMotherEnergy = foundConnector->getCreature()->removeEnergy(givenEnergyToChildGoal / 2.f);

    child->getCreature()->setEnergy(givenFatherEnergy + givenMotherEnergy);
    connectors.emplace_back(child);

    addedCreatures.emplace_back(child->getCreature());
}



void Farm::populationControl() {
    std::chrono::system_clock::time_point start = std::chrono::system_clock::now();

    if (this->connectors.size() > int(INITIAL_CREATURE_COUNT / 2) - (INITIAL_CREATURE_COUNT * 0.05)) {
        std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_time = end - start;
        dataAnalyser.getPopulationControlTime()->addValue(elapsed_time.count());
        return;
    }



    random_device rd;
    mt19937 mt(rd());
    uniform_real_distribution<double> distWidth(0, FARM_WIDTH);
    uniform_real_distribution<double> distHeight(0, FARM_HEIGHT);

    std::vector<BrainConnector *> sortedConnectors = getScoreSortedCreatures();
    int selectedParentCount = sortedConnectors.size() / 2;

    int newConnectorNeeded = int(INITIAL_CREATURE_COUNT / 2) + (INITIAL_CREATURE_COUNT * 0.05) - this->connectors.size();

    float totalEnergyRemoved = 0.f;
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

        totalEnergyRemoved += child->getCreature()->getEnergy();


        connectors.emplace_back(child);
        addedCreatures.emplace_back(child->getCreature());
    }

    removeEnergyFromFarm(totalEnergyRemoved);
    clearDeletedEntities();



    std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_time = end - start;
    dataAnalyser.getPopulationControlTime()->addValue(elapsed_time.count());
}

void Farm::vegetalisation() {
    std::chrono::system_clock::time_point start = std::chrono::system_clock::now();

    random_device rd;
    mt19937 mt(rd());
    uniform_real_distribution<double> distWidth(0, FARM_WIDTH);
    uniform_real_distribution<double> distHeight(0, FARM_HEIGHT);

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



    std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_time = end - start;
    dataAnalyser.getVegetalisationTime()->addValue(elapsed_time.count());
}

void Farm::aTickHavePassed() {
    for (int it = 0; it < connectors.size(); it++) {
        connectors.at(it)->getCreature()->aTickHavePassed();
    }
    for (int it = 0; it < foods.size(); it++) {
        foods.at(it)->aTickHavePassed();
    }
    tickCount++;
}



void Farm::statistics() {
//    if (tickCount % 10 != 0) return;

    std::vector<BrainConnector *> sortedConnectors = getScoreSortedCreatures();
    int populationSize = sortedConnectors.size();

    dataAnalyser.getPopulation()->addValue(populationSize);

    double totalPopulationScore = 0.0;
    for (int it = 0; it < populationSize; it++) {
        totalPopulationScore += sortedConnectors.at(it)->getCreature()->getAge();
    }

    double averagePopulationAge = totalPopulationScore / double(populationSize);

    double maxScore = getScoreSortedCreatures().at(0)->getCreature()->getAge();

    double firstQuartileScore = getScoreSortedCreatures().at(populationSize / 4)->getCreature()->getAge();
    double median = getScoreSortedCreatures().at(populationSize / 2)->getCreature()->getAge();
    double lastQuartileScore = getScoreSortedCreatures().at((3 * populationSize) / 4)->getCreature()->getAge();


    dataAnalyser.getAverageScore()->addValue(averagePopulationAge);
    dataAnalyser.getBestScore()->addValue(maxScore);
    dataAnalyser.getFirstQuartileScore()->addValue(firstQuartileScore);
    dataAnalyser.getMedianScore()->addValue(median);
    dataAnalyser.getLastQuartileScore()->addValue(lastQuartileScore);


    double totalTime = 0.0;
    totalTime += dataAnalyser.getEntityGridTime()->getLastValue();
    totalTime += dataAnalyser.getBrainProcessingTime()->getLastValue();
    totalTime += dataAnalyser.getBrainOutputsTime()->getLastValue();
    totalTime += dataAnalyser.getPrepareActionsTime()->getLastValue();
    totalTime += dataAnalyser.getExecuteActionsTime()->getLastValue();
    totalTime += dataAnalyser.getMoveCreaturesTime()->getLastValue();
    totalTime += dataAnalyser.getPopulationControlTime()->getLastValue();
    totalTime += dataAnalyser.getVegetalisationTime()->getLastValue();

    dataAnalyser.getTotalTime()->addValue(totalTime);



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

    int totalEnergy = availableEnergy + totalFoodsEnergy + totalCreaturesEnergy;
    dataAnalyser.getTotalEnergy()->addValue(totalEnergy);
    dataAnalyser.getAvailableEnergy()->addValue(availableEnergy);
    dataAnalyser.getFoodEnergy()->addValue(totalFoodsEnergy);
    dataAnalyser.getCreaturesEnergy()->addValue(totalCreaturesEnergy);

//
//
//    std::cout << " total: " << getHumanReadableEnergy(totalEnergy);
//    std::cout << "  --  available: " << getHumanReadableEnergy(availableEnergy);
//    std::cout << "  --  creatures: " << getHumanReadableEnergy(totalCreaturesEnergy);
//    std::cout << "  --  foods: " << getHumanReadableEnergy(totalFoodsEnergy);
//    std::cout << "  --  raw total: " << totalEnergy;


}





void Farm::removeEnergyFromFarm(double amount) {
    if (availableEnergy >= amount) {
        availableEnergy -= amount;
        return;
    }


    double totalCollected = 0.0;

    for (int jt = 0; jt < foods.size(); jt++) {
        if (totalCollected >= amount)
            continue;

        if (foods.at(jt)->getEnergy() <= 0) {
            continue;
        }

        totalCollected += foods.at(jt)->removeEnergy(foods.at(jt)->getEnergy());

        toDelete.emplace_back(foods.at(jt));
    }

    if (totalCollected > amount) {
        availableEnergy += totalCollected - amount;
    }
}

bool Farm::isEntityAboutToBeDeleted(int id) {
    for (int jt = 0; jt < toDelete.size(); jt++) {
        if (toDelete.at(jt)->getId() == id) {
            return true;
        }
    }
    return false;
}

void Farm::generateEntityGrid() {
    std::chrono::system_clock::time_point start = std::chrono::system_clock::now();

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



    std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_time = end - start;
    dataAnalyser.getEntityGridTime()->addValue(elapsed_time.count());
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

std::vector<Entity *> Farm::getAccessibleEntities(Creature * creature) {
    std::vector<Entity *> accessibleEntities;
    for (int jt = 0; jt < creature->getSelectedChunks().size(); jt++) {
        Point currentChunk = creature->getSelectedChunks().at(jt);

        std::vector<Entity *> chunkEntities = entityGrid.at(currentChunk.getX()).at(currentChunk.getY());
        accessibleEntities.insert(accessibleEntities.end(), chunkEntities.begin(), chunkEntities.end());
    }
    return accessibleEntities;

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

void Farm::addConnector(BrainConnector * connector) {
    this->connectors.push_back(connector);
}

const vector<Entity *> &Farm::getToDelete() const {
    return toDelete;
}

void Farm::clearAddedCreatures() {
    this->addedCreatures.clear();
}
void Farm::clearAddedEntities() {
    this->addedEntity.clear();
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

const DataAnalyser &Farm::getDataAnalyser() const {
    return dataAnalyser;
}

void Farm::setDataAnalyser(const DataAnalyser &dataAnalyser) {
    Farm::dataAnalyser = dataAnalyser;
}

const vector<std::vector<std::vector<Entity *>>> &Farm::getEntityGrid() const {
    return entityGrid;
}

