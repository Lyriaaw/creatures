//
// Created by Amalric Lombard de Buffières on 8/9/20.
//

#include <unistd.h>
#include "Life.h"
#include "muscles/externals/Mouth.h"
#include "muscles/internals/Movement.h"
#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <thread>


Life::Life(int farmId) : farmId(farmId) {}

void Life::saveAccessibleEntities(std::vector<Entity *> availableEntities, std::vector<Tile *> availableTiles) {
    this->currentAccessibleEntities = availableEntities;
    this->currentAccessibleTiles = availableTiles;
}

void Life::processSensors() {

    for (int it = 0; it < sensors.size(); it++) {
        sensors.at(it)->fetchSensorValue(currentAccessibleEntities, currentAccessibleTiles);
        sensors.at(it)->passValueToNeuron();
    }
}


void Life::processBrain() {
    for (int it = 0; it < brain->getLinks().size(); it++) {
        brain->getLinks().at(it)->getOutput()->clear();
    }

    for (int it = 0; it < brain->getLinksGrid().size(); it++) {
        std::vector<Link *> linksLine = brain->getLinksGrid().at(it);

        for (int jt = 0; jt < linksLine.size(); jt++) {
            Link * link = linksLine.at(jt);

            float inputValue = link->getInput()->getValue();
            float finalValue = inputValue * link->getWeight();
            link->getOutput()->addValue(finalValue);
        }
    }
}
std::vector<ActionDTO> Life::executeExternalActions() {
    std::vector<ActionDTO> currentCreatureActions;
    for (int it = 0; it < externalMuscles.size(); it++) {
        std::vector<ActionDTO> muscleActions = externalMuscles.at(it)->prepareActionDTO(currentAccessibleEntities);

        currentCreatureActions.insert(currentCreatureActions.begin(), muscleActions.begin(), muscleActions.end());
    }
    return currentCreatureActions;
}

std::vector<Entity *> Life::executeInternalActions() {
    std::vector<Entity * > entitiesCreated;
    for (int it = 0; it < internalMuscles.size(); it++) {
        std::vector<Entity * > muscleEntities = internalMuscles.at(it)->executeAction();
        entitiesCreated.insert(entitiesCreated.begin(), muscleEntities.begin(), muscleEntities.end());
    }
    return entitiesCreated;
}

double Life::giveawayEnergy() {
    double muscleEnergy = 0.0;

    for (int it = 0; it < externalMuscles.size(); it++) {
        muscleEnergy += externalMuscles.at(it)->getAndClearEnergyConsumption();
    }

    double sensorEnergy = sensors.size();
    double biasEnergy = 1;
    double wastedEnergyCarrying = 0.01 * energyCenter->getWastedEnergy();

    double usedEnergy = sensorEnergy + biasEnergy + muscleEnergy + wastedEnergyCarrying;

    usedEnergy *= (entity->getAge() / 10000.0);


    while (!entity->tryLockInteraction()) {
        usleep(5000);
    }


    double currentEntityEnergy = this->energyCenter->getAvailableEnergy();


    if (currentEntityEnergy - usedEnergy < 0) {
        usedEnergy = this->energyCenter->getAvailableEnergy();
    }

    if (usedEnergy < 0) {
        std::cout << "Returned negative amount of energy: " << usedEnergy;
        std::cout << " Entity energy: " << this->entity->getMass();
        std::cout << " Muscle Energy: " << muscleEnergy;
        std::cout << std::endl;
    }

    this->energyCenter->setAvailableEnergy(currentEntityEnergy - usedEnergy);

    entity->unlockInteraction();

    return usedEnergy;
}

void Life::processSelectedChunks(){
    for (int it = 0; it < sensors.size(); it++) {
        sensors.at(it)->findSelectedChunks();
    }
    for (int it = 0; it < externalMuscles.size(); it++) {
        externalMuscles.at(it)->findSelectedChunks();
    }

}

std::vector<Point> Life::getSelectedChunks() {
    std::vector<Point> selectedChunks;

    int sensorSize = sensors.size();

    for (int it = 0; it < sensorSize; it++) {
        std::vector<Point> sensorSelectedChunks = sensors.at(it)->getSelectedChunks();

        for (int jt = 0; jt < sensorSelectedChunks.size(); jt++) {
            // If the chunk have not already been selected, select it
            bool found(false);
            int index = 0;
            while(!found && index < selectedChunks.size()) {
                if (selectedChunks.at(index).equals(sensorSelectedChunks.at(jt))) {
                    found = true;
                }
                index++;
            };

            if (!found) {
                selectedChunks.emplace_back(sensorSelectedChunks.at(jt));
            }
        }
    }


    for (int it = 0; it < externalMuscles.size(); it++) {
        std::vector<Point> sensorSelectedChunks = externalMuscles.at(it)->getSelectedChunks();

        for (int jt = 0; jt < sensorSelectedChunks.size(); jt++) {
            // If the chunk have not already been selected, select it
            bool found(false);
            int index = 0;
            while(!found && index < selectedChunks.size()) {
                if (selectedChunks.at(index).equals(sensorSelectedChunks.at(jt))) {
                    found = true;
                }
                index++;
            };

            if (!found) {
                selectedChunks.emplace_back(sensorSelectedChunks.at(jt));
            }
        }
    }




    return selectedChunks;
}

void Life::saveToMongo(MongoClient * client) {
    auto builder = bsoncxx::builder::stream::document{};

    auto array_builder = bsoncxx::builder::basic::array{};
    for (int it = 0; it < parentsIds.size(); it++) {
        array_builder.append(parentsIds[it]);
    }

    builder
        << "farmId" << farmId
        << "id" << this->entity->getId()
        << "birth_tick" << this->birthTick
        << "hue" << this->entity->getColor()
        << "naturalMating" << this->naturalMating
        << "parentsIds" << array_builder;






    bsoncxx::document::value doc_value = builder << bsoncxx::builder::stream::finalize;


    try {
        mongoId = client->saveLife(doc_value.view());

    } catch (const std::exception& e) {
        std::cout << "Error while saving creature: " << entity->getId() << " in thread " << std::this_thread::get_id() << " -> " << e.what() <<  std::endl;
    }
}


void Life::recordDeathToMongo(MongoClient * client, int tick) {
    auto builder = bsoncxx::builder::stream::document{};

    builder << "$set" << bsoncxx::builder::stream::open_document << "death_tick" << tick << bsoncxx::builder::stream::close_document;

    bsoncxx::document::value doc_value = builder << bsoncxx::builder::stream::finalize;


    try {
        client->updateLife(doc_value.view(), farmId, this->entity->getId());

    } catch (const std::exception& e) {
        std::cout << "Error while saving creature: " << entity->getId() << " in thread " << std::this_thread::get_id() << " -> " << e.what() <<  std::endl;
    }
}





nlohmann::json Life::initialDataJson() {
    nlohmann::json life;

    life["id"] = entity->getId();
    life["hue"] = entity->getColor();
    life["naturalMating"] = naturalMating;
    life["parentsIds"] = parentsIds;

    nlohmann::json externalMusclesJSON = nlohmann::json::array();
    for (int it = 0; it < externalMuscles.size(); it++) {
        externalMusclesJSON[it] = externalMuscles.at(it)->toJson();
    }

    life["externalMuscles"] = externalMusclesJSON;


    return life;
}

nlohmann::json Life::updateDataJson() {
    nlohmann::json life;

    life["id"] = entity->getId();
    life["age"] = entity->getAge();
    life["alive"] = isAlive();
//    life["childrenIds"] = childrenIds;
    life["x"] = entity->getPosition().getX();
    life["y"] = entity->getPosition().getY();
    life["size"] = entity->getSize();
    life["rotation"] = entity->getRotation();

    return life;
}

nlohmann::json Life::deathDataJSON() {
    nlohmann::json life;

    life["id"] = entity->getId();
    life["age"] = entity->getAge();

    return life;
}





Entity *Life::getEntity() const {
    return entity;
}

Brain *Life::getBrain() const {
    return brain;
}

void Life::setEntity(Entity *entity) {
    Life::entity = entity;
}

void Life::setBrain(Brain *brain) {
    Life::brain = brain;
}


void Life::addSensor(Sensor * sensor) {
    this->sensors.emplace_back(sensor);
}
void Life::addInternalMuscle(InternalMuscle * muscle) {
    this->internalMuscles.emplace_back(muscle);
}
void Life::addExternalMuscle(ExternalMuscle * muscle) {
    this->externalMuscles.emplace_back(muscle);
}

void Life::connectSensorAndMuscles() {
    for (int it = 0; it < internalMuscles.size(); it++) {
        internalMuscles.at(it)->setEntity(entity);
    }
    for (int it = 0; it < externalMuscles.size(); it++) {
        externalMuscles.at(it)->setEntity(entity);
    }
    for (int it = 0; it < sensors.size(); it++) {
        sensors.at(it)->setEntity(entity);
    }

}

const std::vector<Sensor *> &Life::getSensors() const {
    return sensors;
}

const std::vector<InternalMuscle *> &Life::getInternalMuscles() const {
    return internalMuscles;
}

const std::vector<ExternalMuscle *> &Life::getExternalMuscles() const {
    return externalMuscles;
}

EnergyCenter *Life::getEnergyCenter() const {
    return energyCenter;
}

void Life::setEnergyCenter(EnergyCenter *energyCenter) {
    Life::energyCenter = energyCenter;
}

double Life::addEnergy(double energyToAdd) {
    return energyCenter->addEnergy(energyToAdd);
}

bool Life::isAlive() {
    return energyCenter->getAvailableEnergy() > 0 && entity->isExists();
}


const std::vector<Entity *> &Life::getCurrentAccessibleEntities() const {
    return currentAccessibleEntities;
}

const std::vector<Tile *> &Life::getCurrentAccessibleTiles() const {
    return currentAccessibleTiles;
}

void Life::addChild(int childId) {
    childrenIds.emplace_back(childId);
}
void Life::addParent(int parentId) {
    parentsIds.emplace_back(parentId);
}


const std::vector<int> &Life::getParentsIds() const {
    return parentsIds;
}

const std::vector<int> &Life::getChildrenIds() const {
    return childrenIds;
}

void Life::setNaturalMating(bool naturalMating) {
    Life::naturalMating = naturalMating;
}

int Life::getBirthTick() const {
    return birthTick;
}

void Life::setBirthTick(int birthTick) {
    Life::birthTick = birthTick;
}



