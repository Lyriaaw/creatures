//
// Created by Amalric Lombard de Buffières on 8/15/20.
//

#ifndef CREATURES_ENERGYMANAGEMENT_H
#define CREATURES_ENERGYMANAGEMENT_H


#include "../entities/Entity.h"

class EnergyManagement {
private:
    Entity * body;

    double maxMass;

    double energy;

public:
    double getEnergy() const;
    double setEnergy(double energy);
    double addEnergy(double addedEnergy);
    double removeEnergy(double removedEnergy);
    double getMaxEnergy();

    double getMaxMass() const;

    void setMaxMass(double maxMass);

    bool isAlive();


};


#endif //CREATURES_ENERGYMANAGEMENT_H
