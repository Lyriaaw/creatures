//
// Created by Amalric Lombard de Buffières on 8/12/20.
//

#include "DuplicationMuscle.h"

std::vector<ActionDTO> DuplicationMuscle::prepareActionDTO(std::vector<Entity *> accessibleEntities) {
    std::vector<ActionDTO> actions;

    if (this->neurons.at(0)->getValue() < 0.5f) {
        return actions;
    }

    ActionDTO action = ActionDTO(entity->getId(), 0, "DUPLICATE");
    actions.emplace_back(action);

    return actions;
}

DuplicationMuscle::DuplicationMuscle(float rotation, Entity *entity) : ExternalMuscle(rotation, entity) {

}

std::string DuplicationMuscle::getName() {
    return "DUPLICATION";
}
