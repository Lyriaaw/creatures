//
// Created by Amalric Lombard de Buffières on 8/10/20.
//

#ifndef CREATURES_MOUTH_H
#define CREATURES_MOUTH_H


#include "ExternalMuscle.h"

class Mouth: public ExternalMuscle {
public:
    Mouth(float rotation, Entity *entity);

    std::vector<ActionDTO> prepareActionDTO(std::vector<Entity *> accessibleEntities) override;
};


#endif //CREATURES_MOUTH_H
