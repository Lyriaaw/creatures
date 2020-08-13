//
// Created by Amalric Lombard de Buffières on 8/12/20.
//

#ifndef CREATURES_DUPLICATIONMUSCLE_H
#define CREATURES_DUPLICATIONMUSCLE_H


#include "ExternalMuscle.h"

class DuplicationMuscle: public ExternalMuscle {
public:
    DuplicationMuscle(float rotation, Entity *entity);

    std::vector<ActionDTO> prepareActionDTO(std::vector<Entity *> accessibleEntities) override;

    std::string getName() override;
};


#endif //CREATURES_DUPLICATIONMUSCLE_H
