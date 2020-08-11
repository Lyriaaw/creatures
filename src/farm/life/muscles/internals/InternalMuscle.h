//
// Created by Amalric Lombard de Buffières on 8/9/20.
//

#ifndef CREATURES_INTERNALMUSCLE_H
#define CREATURES_INTERNALMUSCLE_H


#include "../Muscle.h"

class InternalMuscle: public Muscle {
public:
    InternalMuscle(float rotation, Entity *entity);

    virtual std::vector<Entity *> executeAction() = 0;
};


#endif //CREATURES_INTERNALMUSCLE_H
