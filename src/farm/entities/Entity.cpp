//
// Created by Amalric Lombard de Buffières on 7/16/20.
//

#include "Entity.h"
#include <iostream>

int Entity::GLOBAL_INDEX = 1;

Entity::Entity(Point position): position(position), mass(0.0), size(0), age(0) {
    this->id = GLOBAL_INDEX;
    GLOBAL_INDEX++;
    this->exists = true;
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
    Point simpleCoordinates = {float(int(this->getPosition().getX() / float(CHUNK_SIZE))), float(int(this->getPosition().getY() / float(CHUNK_SIZE)))};

    if (simpleCoordinates.getX() < 0 || simpleCoordinates.getX() >= CHUNK_COUNT_WIDTH || simpleCoordinates.getY() < 0 || simpleCoordinates.getY() >= CHUNK_COUNT_HEIGHT) {
        std::cout << "Error while generating chunk coordinates !" << std::endl;
        std::cout << "X: " << simpleCoordinates.getX() << " Y: " << simpleCoordinates.getY() << " mapX: " << position.getX() << " mapY: " << position.getY() << std::endl;
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

void Entity::setSize(float size) {
    Entity::size = size;
}

bool Entity::isExists() const {
    return exists;
}

void Entity::setExists(bool exists) {
    Entity::exists = exists;
}

float Entity::getMass() const {
    return mass;
}

double Entity::setMass(double mass) {
    Entity::mass = mass;
}

double Entity::addMass(double addedMass) {
    double remaining = 0.f;

    double newMass = this->mass + addedMass;

    if (newMass > getMaxMass()) {
        remaining = newMass - getMaxMass();
        newMass = newMass - remaining;
    }

    this->mass = newMass;

    return remaining;
}

double Entity::removeMass(double removedMass) {
    double newMass = this->mass - removedMass;
    double returned = removedMass;

    if (newMass <= 0) {
        returned = this->mass;
        this->mass = 0;
    }

    this->mass = newMass;

    return returned;
}

double Entity::getMaxMass() {
//    double result = this->size * pow(299792458, 2);
    double result = this->size * 1000;
    return result;
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
