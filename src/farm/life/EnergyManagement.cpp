//
// Created by Amalric Lombard de Buffières on 8/15/20.
//

#include "EnergyManagement.h"


double EnergyManagement::getEnergy() const {
    return energy;
}

double EnergyManagement::setEnergy(double energy) {
    EnergyManagement::energy = energy;
}

double EnergyManagement::addEnergy(double addedEnergy) {
    double remaining = 0.f;

    double newEnergy = this->energy + addedEnergy;

    if (newEnergy > getMaxEnergy()) {
        remaining = newEnergy - getMaxEnergy();
        newEnergy = newEnergy - remaining;
    }

    this->energy = newEnergy;

    return remaining;
}

double EnergyManagement::removeEnergy(double removedEnergy) {
    double newEnergy = this->energy - removedEnergy;
    double returned = removedEnergy;

    if (newEnergy <= 0) {
        returned = this->energy;
        this->energy = 0;
    }

    this->energy = newEnergy;

    return returned;
}

double EnergyManagement::getMaxMass() const {
    return maxMass;
}

void EnergyManagement::setMaxMass(double maxMass) {
    EnergyManagement::maxMass = maxMass;
}

double EnergyManagement::getMaxEnergy() {
    return this->maxMass;
}

bool EnergyManagement::isAlive() {
    return this->body->isExists() && this->energy >= 0;
}
