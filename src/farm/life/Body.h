//
// Created by Amalric Lombard de Buffières on 8/15/20.
//

#ifndef CREATURES_BODY_H
#define CREATURES_BODY_H


#include "../entities/Entity.h"

class Body: public Entity {
private:
    double maximumMass;
    double massTransformationRatio;
    double energy;

public:

    Body(const Point &position);

    double getMaximumMass() const;
    void setMaximumMass(double maximumMass);
    double getMassTransformationRatio() const;
    void setMassTransformationRatio(double massTransformationRatio);


    double getEnergy() const;
    void setEnergy(double energy);
    void addEnergy(double energy);
    double removeEnergy(double removedEnergy);
};


#endif //CREATURES_BODY_H
