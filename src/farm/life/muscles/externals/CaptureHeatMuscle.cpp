//
// Created by Amalric Lombard de Buffières on 8/13/20.
//

#include "CaptureHeatMuscle.h"

std::vector<ActionDTO> CaptureHeatMuscle::prepareActionDTO(std::vector<Entity *> accessibleEntities) {
    std::vector<ActionDTO> actions;

    if (this->neurons.at(0)->getValue() < 0.5f) {
        return actions;
    }

    ActionDTO action = ActionDTO(entity->getId(), 0, "CAPTURE_HEAT", (this->neurons.at(0)->getValue() - 0.5) * 2.0);
    actions.emplace_back(action);

    return actions;
}

CaptureHeatMuscle::CaptureHeatMuscle(float rotation, Entity *entity) : ExternalMuscle(rotation, entity) {

}

std::string CaptureHeatMuscle::getName() {
    return "CAPTURE_HEAT";
}
