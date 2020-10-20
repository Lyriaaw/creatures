//
// Created by Amalric Lombard de Buffières on 10/20/20.
//

#include "PheromoneEmitter.h"
std::vector<ActionDTO> PheromoneEmitter::prepareActionDTO(std::vector<Entity *> accessibleEntities) {
    std::vector<ActionDTO> actions;

    energyConsumption += abs(this->neurons.at(0)->getValue() * 10);

    if (this->neurons.at(0)->getValue() < 0.95f) {
        return actions;
    }


    ActionDTO action = ActionDTO(entity->getId(), -1, "EMIT_PHEROMONE");
    action.setPheromoneEmissionColor(color);
    actions.emplace_back(action);

    return actions;
}

PheromoneEmitter::PheromoneEmitter(float rotation, Entity *entity) : ExternalMuscle(rotation, entity) {

}

std::string PheromoneEmitter::getName() {
    return "MOUTH";
}

void PheromoneEmitter::setColor(double color) {
    PheromoneEmitter::color = color;
}
