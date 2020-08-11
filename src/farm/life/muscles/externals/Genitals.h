//
// Created by Amalric Lombard de Buffières on 8/10/20.
//

#ifndef CREATURES_GENITALS_H
#define CREATURES_GENITALS_H


#include "ExternalMuscle.h"

class Genitals: public ExternalMuscle {
public:
    Genitals(float rotation, Entity *entity);

    std::vector<ActionDTO> prepareActionDTO(std::vector<Entity *> accessibleEntities) override;

    std::string getName() override;
};


#endif //CREATURES_GENITALS_H
