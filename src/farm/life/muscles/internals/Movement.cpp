//
// Created by Amalric Lombard de Buffières on 8/10/20.
//

#include "Movement.h"

std::vector<Entity *> Movement::executeAction() {

    float actualSpeed = this->neurons.at(0)->getValue() * this->entity->getSize();

    if (actualSpeed < -1 * (this->entity->getSize() / 2.f)) {
        actualSpeed = -1 * (this->entity->getSize() / 2.f);
    }

    energyConsumption += (1/2) * this->entity->getSize() * pow(this->neurons.at(0)->getValue(), 2);

    Point currentEntityPosition = this->entity->getPosition();


    float nextX = currentEntityPosition.getX() + (actualSpeed * cos((this->entity->getRotation() * M_PI)));
    float nextY = currentEntityPosition.getY() + (actualSpeed * sin((this->entity->getRotation() * M_PI)));



    if (nextX < 0) {
        nextX = 0;
    }
    if (nextX >= FARM_WIDTH) {
        nextX = FARM_WIDTH - 1;
    }
    if (nextY < 0) {
        nextY = 0;
    }
    if (nextY >= FARM_HEIGHT) {
        nextY = FARM_HEIGHT - 1;
    }

    currentEntityPosition.setX(nextX);
    currentEntityPosition.setY(nextY);

    this->entity->setPosition(currentEntityPosition);

    std::vector<Entity *> empty;
    return empty;
}

Movement::Movement(float rotation, Entity *entity) : InternalMuscle(rotation, entity), speed(0.f) {

}

std::string Movement::getName() {
    return "Movement";
}

