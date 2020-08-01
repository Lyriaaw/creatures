//
// Created by Amalric Lombard de Buffières on 8/1/20.
//

#include <iostream>
#include "Creature.h"
#include "../World.h"

Creature::Creature(Point position, float size): Entity(position, size) {
    this->color = (rand() % 1000) / 1000.f;
    this->brightness = 0.5f;
}

void Creature::move() {
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


    float nextX = this->position.getX() + (this->speed * cos(this->rotation));
    float nextY = this->position.getY() + (this->speed * sin(this->rotation));



    if (this->position.getX() < 0) {
        nextX = 0;
    }
    if (this->position.getX() > FARM_WIDTH) {
        nextX = FARM_WIDTH;
    }

    if (this->position.getY() < 0) {
        nextY = 0;
    }
    if (this->position.getY() > FARM_HEIGHT) {
        nextY = FARM_HEIGHT;
    }

    this->position.setX(nextX);
    this->position.setY(nextY);
}