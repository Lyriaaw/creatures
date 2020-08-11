//
// Created by Amalric Lombard de Buffières on 8/10/20.
//

#include "Rotation.h"

std::vector<Entity *> Rotation::executeAction() {
    this->entity->setRotation(this->neurons.at(0)->getValue());

    std::vector<Entity *> empty;
    return empty;
}

Rotation::Rotation(float rotation, Entity *entity) : InternalMuscle(rotation, entity) {

}
