//
// Created by Amalric Lombard de Buffières on 8/10/20.
//

#include "Rotation.h"

std::vector<Entity *> Rotation::executeAction() {

    float currentEntityRotation = this->entity->getRotation();
    float newRotation = currentEntityRotation + this->neurons.at(0)->getValue();

    if (newRotation < - 2 * M_PI) {
        newRotation += 2 * M_PI;
    }

    if (newRotation > 2 * M_PI) {
        newRotation -= 2 * M_PI;
    }

    this->entity->setRotation(newRotation);

    std::vector<Entity *> empty;
    return empty;
}

Rotation::Rotation(float rotation, Entity *entity) : InternalMuscle(rotation, entity) {

}

std::string Rotation::getName() {
    return "Rotation";
}
