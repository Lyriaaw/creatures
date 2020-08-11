//
// Created by Amalric Lombard de Buffières on 8/10/20.
//

#ifndef CREATURES_ROTATION_H
#define CREATURES_ROTATION_H


#include "InternalMuscle.h"

class Rotation: public InternalMuscle {
public:
    Rotation(float rotation, Entity *entity);

    std::vector<Entity *> executeAction() override;
};


#endif //CREATURES_ROTATION_H
