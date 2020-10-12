//
// Created by Amalric Lombard de Buffières on 10/12/20.
//

#include "EnergyCenter.h"

double EnergyCenter::addEnergy(double energyToAdd) {
    double massConverted = 0.0;

    if (entity->getMass() < maxMass) {
        if (0.1 * energyToAdd < maxMass) {
            massConverted = 0.1 * energyToAdd;
        }
    }

    entity->setMass(entity->getMass() + massConverted);
    double wasted = 0.0;

    double newAvailableEnergy = availableEnergy + (energyToAdd - massConverted);
    if (newAvailableEnergy > maxMass) {
        wasted = newAvailableEnergy - maxMass;
        newAvailableEnergy = maxMass;
    }

    this->availableEnergy = newAvailableEnergy;
    this->wastedEnergy += wasted;

    return 0.0;
}

double EnergyCenter::getAvailableEnergy() const {
    return availableEnergy;
}

void EnergyCenter::setAvailableEnergy(double availableEnergy) {
    EnergyCenter::availableEnergy = availableEnergy;
}

double EnergyCenter::getMaxMass() const {
    return maxMass;
}

void EnergyCenter::setMaxMass(double maxMass) {
    EnergyCenter::maxMass = maxMass;
}

Entity *EnergyCenter::getEntity() const {
    return entity;
}

void EnergyCenter::setEntity(Entity *entity) {
    EnergyCenter::entity = entity;
}

EnergyCenter::EnergyCenter(Entity *entity) : entity(entity), availableEnergy(0.0), maxMass(0.0), wastedEnergy(0.0) {}

double EnergyCenter::getWastedEnergy() const {
    return wastedEnergy;
}

void EnergyCenter::setWastedEnergy(double wastedEnergy) {
    EnergyCenter::wastedEnergy = wastedEnergy;
}
