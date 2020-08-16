//
// Created by Amalric Lombard de Buffières on 8/9/20.
//

#include <iostream>
#include <zconf.h>
#include "Tile.h"

double Tile::getHeight() const {
    return height;
}

void Tile::setHeight(double height) {
    Tile::height = height;
}

double Tile::getHeat() const {
    return heat;
}

void Tile::setHeat(double heat) {
    Tile::heat = heat;
}

double Tile::getGround() const {
    return ground;
}

void Tile::setGround(double ground) {
    if (ground < 0) {
        std::cout << "Setting ground to " << ground << std::endl;
    }
    std::lock_guard<std::mutex> guard(add_ground_mutex);
    this->ground = ground;
}
double Tile::removeGround(double removedGround) {
    if (ground - removedGround < 0) {
        std::cout << "Error removing ground : " << ground << " - " << removedGround << std::endl;
    }
    std::lock_guard<std::mutex> guard(add_ground_mutex);
    this->ground -= removedGround;

    return removedGround;
}
double Tile::lockOwnerRemoveGround(double removedGround) {
    if (ground - removedGround < 0) {
        std::cout << "Error removing ground : " << ground << " - " << removedGround << std::endl;
    }
    this->ground -= removedGround;
    return removedGround;
}

void Tile::addGround(double value) {
    std::lock_guard<std::mutex> guard(add_ground_mutex);
    addedGround += value;
}

void Tile::addHeat(double value) {
    std::lock_guard<std::mutex> guard(add_heat_mutex);
    addedHeat += value;
}

void Tile::processAddedHeat() {
    heat += addedHeat;
    addedHeat = 0;
}

void Tile::processAddedGround() {
    ground += addedGround;
    addedGround = 0;
}



void Tile::addHeight(double value) {
    height += value;
}

Tile::Tile(const Point &position) : position(position), heat(0.0), ground(0.0), height(0.0), addedHeat(0.0), addedGround(0.0) {}

const Point &Tile::getPosition() const {
    return position;
}

double Tile::getAddedHeat() const {
    return addedHeat;
}

double Tile::getAddedGround() const {
    return addedGround;
}

void Tile::lockGroundMutex() {
    while (!add_ground_mutex.try_lock()) {
        usleep(10);
    }
}

void Tile::unlockGroundMutex() {
    add_ground_mutex.unlock();
}

