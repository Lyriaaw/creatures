//
// Created by Amalric Lombard de Buffières on 8/10/20.
//

#include "Mouth.h"

std::vector<ActionDTO> Mouth::prepareActionDTO(std::vector<Entity *> accessibleEntities) {
    std::vector<ActionDTO> actions;

    if (this->neurons.at(0)->getValue() < 0.5f) {
        return actions;
    }

    energyConsumption += this->neurons.at(0)->getValue();

    float mouthTotalRotation = (float(rotation) + entity->getRotation()) * float(M_PI);

    float mouthSize = entity->getSize() / 3.f;
    float mouthX = (cos(mouthTotalRotation) * entity->getSize()) + entity->getPosition().getX();
    float mouthY = (sin(mouthTotalRotation) * entity->getSize()) + entity->getPosition().getY();

    Entity * closestEntity = nullptr;
    float smallestDistance = FARM_WIDTH;

    for (int it = 0; it < accessibleEntities.size(); it++) {
        float distanceX = abs(accessibleEntities.at(it)->getPosition().getX() - mouthX);
        float distanceY = abs(accessibleEntities.at(it)->getPosition().getY() - mouthY);

        float distance = sqrt(pow(distanceX, 2) + pow(distanceY, 2));

        if (distance <= mouthSize) {
            if (distance < smallestDistance) {
                smallestDistance = distance;
                closestEntity = accessibleEntities.at(it);
            }
        }
    }

    if (closestEntity == nullptr) {
        return actions;
    }

    if (closestEntity->getSize() <= mouthSize) {
        ActionDTO action = ActionDTO(entity->getId(), closestEntity->getId(), "EAT");
        actions.emplace_back(action);
    }
    return actions;
}

Mouth::Mouth(float rotation, Entity *entity) : ExternalMuscle(rotation, entity) {

}
