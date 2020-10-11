//
// Created by Amalric Lombard de Buffières on 8/10/20.
//

#include "Movement.h"

std::vector<Entity *> Movement::executeAction() {

    float actualSpeed = this->neurons.at(0)->getValue() * this->entity->getSize();

    if (actualSpeed < -1 * (this->entity->getSize() / 2.f)) {
        actualSpeed = -1 * (this->entity->getSize() / 2.f);
    }

    energyConsumption += actualSpeed;

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

    Point simpleCoordinates = currentEntityPosition.getSimpleCoordinates();

    if (simpleCoordinates.getX() < 0 || simpleCoordinates.getX() >= CHUNK_COUNT_WIDTH || simpleCoordinates.getY() < 0 || simpleCoordinates.getY() >= CHUNK_COUNT_HEIGHT) {
        std::cout << "ERROR WHILE MOVING !" << std::endl;
        std::cout << "X: " << simpleCoordinates.getX() << " Y: " << simpleCoordinates.getY() << " mapX: " << currentEntityPosition.getX() << " mapY: " << currentEntityPosition.getY() << std::endl;
    }






    this->entity->setPosition(currentEntityPosition);

    std::vector<Entity *> empty;
    return empty;
}

Movement::Movement(float rotation, Entity *entity) : InternalMuscle(rotation, entity) {

}

std::string Movement::getName() {
    return "Movement";
}

