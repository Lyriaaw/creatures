//
// Created by Amalric Lombard de Buffières on 7/16/20.
//

#include "Entity.h"
#include <iostream>

int Entity::GLOBAL_INDEX = 1;

Entity::Entity(Point position, float size): position(position) {
    this->id = GLOBAL_INDEX;
    GLOBAL_INDEX++;
    
    this->size = size;
}



float Entity::getColor() const {
    return color;
}

const Point &Entity::getPosition() const {
    return position;
}

float Entity::getSize() const {
    return size;
}

float Entity::getRotation() const {
    return rotation;
}

float Entity::getSpeed() const {
    return speed;
}

float Entity::getBrightness() const {
    return brightness;
}

void Entity::setPosition(const Point &position) {
    Entity::position = position;
}


Point Entity::getSimpleCoordinates() {
    return {float(int(this->getPosition().getX() / float(CHUNK_SIZE))), float(int(this->getPosition().getY() / float(CHUNK_SIZE)))};
}

int Entity::getId() const {
    return id;
}
