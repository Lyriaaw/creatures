//
// Created by Amalric Lombard de Buffières on 8/13/20.
//

#include "CaptureGroundMuscle.h"

std::vector<ActionDTO> CaptureGroundMuscle::prepareActionDTO(std::vector<Entity *> accessibleEntities) {
    std::vector<ActionDTO> actions;

    if (this->neurons.at(0)->getValue() < 0.5f) {
        return actions;
    }

    ActionDTO action = ActionDTO(entity->getId(), 0, "CAPTURE_GROUND", (this->neurons.at(0)->getValue() - 0.5) * 2.0);
    actions.emplace_back(action);

    return actions;
}

CaptureGroundMuscle::CaptureGroundMuscle(float rotation, Entity *entity) : ExternalMuscle(rotation, entity) {

}

std::string CaptureGroundMuscle::getName() {
    return "CAPTURE_GROUND";
}
