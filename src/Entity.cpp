//
// Created by Amalric Lombard de Buffières on 7/16/20.
//

#include "Entity.h"
#include <iostream>
#include <math.h>

#include "World.h"


Entity::Entity(Point position, float size): position(position) {
    this->size = size;
    this->color = (rand() % 1000) / 1000.f;

}

void Entity::move() {
    float addedSpeed = (float(rand() % 21) - 10.f) / 10.f;
    float addedRotation = (float(rand() % 21) - 10.f) / 100.f;

    this->rotation += addedRotation;
    this->speed += addedSpeed;

    if (this->speed > 5) {
        this->speed = 5;
    }
    if (this->speed < -2.5f) {
        this->speed = -2.5f;
    }



    this->position.setX(this->position.getX() + (this->speed * cos(this->rotation)));
    this->position.setY(this->position.getY() + (this->speed * sin(this->rotation)));

    if (this->position.getX() < 0) {
        this->position.setX(FARM_WIDTH);
    }
    if (this->position.getX() > FARM_WIDTH) {
        this->position.setX(0);
    }

    if (this->position.getY() < 0) {
        this->position.setY(FARM_HEIGHT);
    }
    if (this->position.getY() > FARM_HEIGHT) {
        this->position.setY(0);
    }
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
