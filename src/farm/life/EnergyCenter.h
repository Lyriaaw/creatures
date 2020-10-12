//
// Created by Amalric Lombard de Buffières on 10/12/20.
//

#ifndef CREATURES_ENERGYCENTER_H
#define CREATURES_ENERGYCENTER_H


#include "../entities/Entity.h"

class EnergyCenter {
private:
    double availableEnergy;
    double wastedEnergy;
    double maxMass;

    Entity * entity;

public:

    EnergyCenter(Entity *entity);

    double getAvailableEnergy() const;

    void setAvailableEnergy(double availableEnergy);

    double getMaxMass() const;

    void setMaxMass(double maxMass);

    Entity *getEntity() const;

    void setEntity(Entity *entity);

    double addEnergy(double energyToAdd);

    double getWastedEnergy() const;

    void setWastedEnergy(double wastedEnergy);
};


#endif //CREATURES_ENERGYCENTER_H
