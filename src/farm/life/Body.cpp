//
// Created by Amalric Lombard de Buffières on 8/15/20.
//

#include "Body.h"

double Body::getMaximumMass() const {
    return maximumMass;
}

void Body::setMaximumMass(double maximumMass) {
    Body::maximumMass = maximumMass;
}

double Body::getMassTransformationRatio() const {
    return massTransformationRatio;
}

void Body::setMassTransformationRatio(double massTransformationRatio) {
    Body::massTransformationRatio = massTransformationRatio;
}


void Body::addEnergy(double newEnergy) {
    this->energy = this->energy + newEnergy;

    this->setEnergy(this->energy);
}

double Body::removeEnergy(double removedEnergy) {
    double newEnergy = this->energy - removedEnergy;
    double returned = removedEnergy;

    if (newEnergy <= 0) {
        returned = this->energy;
        this->energy = 0;
    }

    this->energy = newEnergy;

    this->setEnergy(this->energy);

    return returned;
}

double Body::getEnergy() const {
    return energy;
}

void Body::setEnergy(double newEnergy) {
    Body::energy = newEnergy;
}

Body::Body(const Point &position) : Entity(position) {}
