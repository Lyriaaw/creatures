//
// Created by Amalric Lombard de Buffières on 8/9/20.
//

#include <iostream>
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
    Tile::ground = ground;
}

void Tile::addGround(double value) {
    ground += value;
}

void Tile::addHeat(double value) {
    heat += value;
}

void Tile::addHeight(double value) {
    height += value;
}

Tile::Tile(const Point &position) : position(position), heat(0.0), ground(0.0), height(0.0) {}

const Point &Tile::getPosition() const {
    return position;
}
