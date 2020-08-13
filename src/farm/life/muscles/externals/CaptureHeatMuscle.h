//
// Created by Amalric Lombard de Buffières on 8/13/20.
//

#ifndef CREATURES_CAPTUREHEATMUSCLE_H
#define CREATURES_CAPTUREHEATMUSCLE_H


#include "ExternalMuscle.h"

class CaptureHeatMuscle: public ExternalMuscle {
public:
    CaptureHeatMuscle(float rotation, Entity *entity);

    std::vector<ActionDTO> prepareActionDTO(std::vector<Entity *> accessibleEntities) override;

    std::string getName() override;
};


#endif //CREATURES_CAPTUREHEATMUSCLE_H
