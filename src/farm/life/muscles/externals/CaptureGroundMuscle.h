//
// Created by Amalric Lombard de Buffières on 8/13/20.
//

#ifndef CREATURES_CAPTUREGROUNDMUSCLE_H
#define CREATURES_CAPTUREGROUNDMUSCLE_H


#include "ExternalMuscle.h"

class CaptureGroundMuscle: public ExternalMuscle {
public:
    CaptureGroundMuscle(float rotation, Entity *entity);

    std::vector<ActionDTO> prepareActionDTO(std::vector<Entity *> accessibleEntities) override;

    std::string getName() override;
};


#endif //CREATURES_CAPTUREGROUNDMUSCLE_H
