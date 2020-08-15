//
// Created by Amalric Lombard de Buffières on 8/9/20.
//

#include "Life.h"
#include "muscles/externals/Mouth.h"
#include "muscles/internals/Movement.h"

void Life::processSensors(std::vector<Entity *> availableEntities, std::vector<Tile *> availableTiles) {
    for (int it = 0; it < sensors.size(); it++) {
        sensors.at(it)->fetchSensorValue(availableEntities, availableTiles);
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
std::vector<ActionDTO> Life::executeExternalActions(std::vector<Entity *> availableEntities) {
    std::vector<ActionDTO> currentCreatureActions;
    for (int it = 0; it < externalMuscles.size(); it++) {
        std::vector<ActionDTO> muscleActions = externalMuscles.at(it)->prepareActionDTO(availableEntities);

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
    double biasEnergy = 10;
//    double sensorEnergy = 0;
//    double biasEnergy = 0;

    double usedEnergy = sensorEnergy + biasEnergy + muscleEnergy;

    double currentEntityEnergy = this->body->getEnergy();

    if (currentEntityEnergy - usedEnergy < 0) {
        usedEnergy = this->body->getEnergy();
    }

    if (usedEnergy < 0) {
        std::cout << "Returned negative amount of energy: " << usedEnergy << std::endl;
    }

    this->body->setEnergy(currentEntityEnergy - usedEnergy);

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

    for (int it = 0; it < sensors.size(); it++) {
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

Brain *Life::getBrain() const {
    return brain;
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
        internalMuscles.at(it)->setEntity(body);
    }
    for (int it = 0; it < externalMuscles.size(); it++) {
        externalMuscles.at(it)->setEntity(body);
    }
    for (int it = 0; it < sensors.size(); it++) {
        sensors.at(it)->setEntity(body);
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

Body *Life::getBody() const {
    return body;
}

Body *Life::getEntity() const {
    return body;
}

void Life::setEntity(Body * entity) {
    this->body = entity;
}

double Life::addEnergy(double addedEnergy) {
    double remaining = 0.f;

    double newEnergy = this->energy + addedEnergy;

    if (newEnergy > getMaxEnergy()) {
        remaining = newEnergy - getMaxEnergy();
        newEnergy = newEnergy - remaining;
    }

    this->energy = newEnergy;

    return remaining;
}


