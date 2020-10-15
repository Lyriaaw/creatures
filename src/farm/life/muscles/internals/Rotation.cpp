//
// Created by Amalric Lombard de Buffières on 8/10/20.
//

#include "Rotation.h"

std::vector<Entity *> Rotation::executeAction() {
    float currentEntityRotation = this->entity->getRotation();
    float newRotation = currentEntityRotation + this->neurons.at(0)->getValue() / 10.0;

    if (newRotation < -2) {
        newRotation += 2;
    }

    if (newRotation > 2) {
        newRotation -= 2;
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
