//
// Created by Amalric Lombard de Buffières on 8/10/20.
//

#ifndef CREATURES_MOVEMENT_H
#define CREATURES_MOVEMENT_H


#include "InternalMuscle.h"

class Movement: public InternalMuscle  {
public:
    Movement(float rotation, Entity *entity);

    std::vector<Entity *> executeAction() override;

    std::string getName() override;
};


#endif //CREATURES_MOVEMENT_H
