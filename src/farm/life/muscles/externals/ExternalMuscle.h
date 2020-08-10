//
// Created by Amalric Lombard de Buffières on 8/9/20.
//

#ifndef CREATURES_EXTERNALMUSCLE_H
#define CREATURES_EXTERNALMUSCLE_H


#include "../Muscle.h"

class ExternalMuscle: public Muscle {
public:
    ExternalMuscle(float rotation, Entity *entity);

    virtual std::vector<ActionDTO> prepareActionDTO(std::vector<Entity *> accessibleEntities) = 0;

};


#endif //CREATURES_EXTERNALMUSCLE_H
