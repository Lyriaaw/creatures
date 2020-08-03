//
// Created by Amalric Lombard de Buffières on 7/16/20.
//

#include "Entity.h"
#include <iostream>

int Entity::GLOBAL_INDEX = 1;

Entity::Entity(Point position): position(position) {
    this->id = GLOBAL_INDEX;
    GLOBAL_INDEX++;
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

void Entity::setRotation(float rotation) {
    Entity::rotation = rotation;
}

void Entity::setSpeed(float speed) {
    Entity::speed = speed;
}

void Entity::setColor(float color) {
    Entity::color = color;
}

void Entity::setSize(float size) {
    Entity::size = size;
}
