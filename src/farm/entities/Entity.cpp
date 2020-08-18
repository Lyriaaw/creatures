//
// Created by Amalric Lombard de Buffières on 7/16/20.
//

#include "Entity.h"
#include <iostream>

int Entity::GLOBAL_INDEX = 1;

Entity::Entity(Point position): position(position), age(0), rotation(0.0), speed(0.0) {
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
    double value = mass / 1000.f;
    return value;
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
    Point simpleCoordinates = {float(int(this->getPosition().getX() / float(CHUNK_SIZE))), float(int(this->getPosition().getY() / float(CHUNK_SIZE)))};

    if (simpleCoordinates.getX() < 0 || simpleCoordinates.getX() >= CHUNK_COUNT_WIDTH || simpleCoordinates.getY() < 0 || simpleCoordinates.getY() >= CHUNK_COUNT_HEIGHT) {
        std::cout << "Error while generating chunk coordinates !" << std::endl;
        std::cout << "X: " << simpleCoordinates.getX() << " Y: " << simpleCoordinates.getY() << " mapX: " << position.getX() << " mapY: " << position.getY() << std::endl;

        return Point(0,0);
    }

    return simpleCoordinates;
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



bool Entity::isExists() const {
    return this->mass >= 0;
}


int Entity::getAge() const {
    return age;
}

void Entity::setAge(int age) {
    Entity::age = age;
}

void Entity::aTickHavePassed(){
    Entity::age++;
}

void Entity::setBrightness(float brightness) {
    Entity::brightness = brightness;
}

double Entity::getMass() const {
    return mass;
}

void Entity::setMass(double mass) {
    Entity::mass = mass;
}

void Entity::addMass(double addedMass) {
    this->mass += addedMass;
}

