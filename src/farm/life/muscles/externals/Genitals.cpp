//
// Created by Amalric Lombard de Buffières on 8/10/20.
//

#include "Genitals.h"

std::vector<ActionDTO> Genitals::prepareActionDTO(std::vector<Entity *> accessibleEntities) {
    std::vector<ActionDTO> actions;

    energyConsumption += abs(this->neurons.at(0)->getValue() * 10);

    if (this->neurons.at(0)->getValue() < 0.5f) {
        return actions;
    }

//    energyConsumption += this->neurons.at(0)->getValue();


    float genitalsTotalRotation = (float(rotation) + entity->getRotation()) * float(M_PI);

    float genitalsSize = entity->getRotation() / 4.f;
    float genitalsX = (cos(genitalsTotalRotation) * entity->getRotation()) + entity->getPosition().getX();
    float genitalsY = (sin(genitalsTotalRotation) * entity->getRotation()) + entity->getPosition().getY();

    Entity * closestEntity = nullptr;
    float smallestDistance = FARM_WIDTH;

    for (int it = 0; it < accessibleEntities.size(); it++) {
        float distanceX = abs(accessibleEntities.at(it)->getPosition().getX() - genitalsX);
        float distanceY = abs(accessibleEntities.at(it)->getPosition().getY() - genitalsY);

        float distance = sqrt(pow(distanceX, 2) + pow(distanceY, 2));

        if (distance <= genitalsSize) {
            if (distance < smallestDistance) {
                smallestDistance = distance;
                closestEntity = accessibleEntities.at(it);
            }
        }
    }

    if (closestEntity == nullptr) {
        return actions;
    }

    if (smallestDistance <= closestEntity->getSize()) {
        ActionDTO action = ActionDTO(entity->getId(), closestEntity->getId(), "MATE");
        actions.emplace_back(action);
    }
    return actions;
}

Genitals::Genitals(float rotation, Entity *entity) : ExternalMuscle(rotation, entity) {

}

std::string Genitals::getName() {
    return "GENITALS";
}
