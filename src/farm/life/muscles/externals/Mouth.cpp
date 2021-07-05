//
// Created by Amalric Lombard de Buffières on 8/10/20.
//

#include "Mouth.h"

std::vector<ActionDTO> Mouth::prepareActionDTO(std::vector<Entity *> accessibleEntities) {
    std::vector<ActionDTO> actions;

    energyConsumption += abs(this->neurons.at(0)->getValue() * 10);

    if (this->neurons.at(0)->getValue() < -0.5f) {
        ActionDTO action = ActionDTO(entity->getId(), -1, "POOP");
        actions.emplace_back(action);
    }

    if (this->neurons.at(0)->getValue() < 0.5f) {
        return actions;
    }


    float mouthTotalRotation = (float(rotation) + entity->getRotation()) * float(M_PI);

    float mouthSize = entity->getSize() / 3.f;
    float mouthX = (cos(mouthTotalRotation) * entity->getSize()) + entity->getPosition().getX();
    float mouthY = (sin(mouthTotalRotation) * entity->getSize()) + entity->getPosition().getY();

    Entity * closestEntity = nullptr;
    float smallestDistance = FARM_WIDTH;

    for (int it = 0; it < accessibleEntities.size(); it++) {
        if (accessibleEntities.at(it)->getId() == entity->getId()) {
            continue;
        }
        float distanceX = abs(accessibleEntities.at(it)->getPosition().getX() - mouthX);
        float distanceY = abs(accessibleEntities.at(it)->getPosition().getY() - mouthY);

        float distance = sqrt(pow(distanceX, 2) + pow(distanceY, 2));

        if (distance <= mouthSize + accessibleEntities.at(it)->getSize()) {
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
        ActionDTO action = ActionDTO(entity->getId(), closestEntity->getId(), "EAT_ENTITY");
        action.setSubject(closestEntity);
        if (closestEntity->isLife()) {
            action.setType("EAT_LIFE");
        }
        actions.emplace_back(action);
    } else {

        ActionDTO action  = ActionDTO(entity->getId(), closestEntity->getId(), "BITE_ENTITY");
        if (closestEntity->isLife()) {
            action.setType("BITE_LIFE");
        }

        action.setSubject(closestEntity);

        actions.emplace_back(action);
    }

    return actions;
}

Mouth::Mouth(float rotation, Entity *entity) : ExternalMuscle(rotation, entity) {

}

std::string Mouth::getName() {
    return "MOUTH";
}

nlohmann::json Mouth::toJson() {
    nlohmann::json json;

    json["type"] = "genitals";
    json["rotation"] = rotation;
    json["size"] = 0.33;

    return json;
}
