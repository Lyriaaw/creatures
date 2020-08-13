//
// Created by Amalric Lombard de Buffières on 8/9/20.
//

#include "Tile.h"

float Tile::getHeight() const {
    return height;
}

void Tile::setHeight(float height) {
    Tile::height = height;
}

float Tile::getHeat() const {
    return heat;
}

void Tile::setHeat(float heat) {
    Tile::heat = heat;
}

float Tile::getGround() const {
    return ground;
}

void Tile::setGround(float ground) {
    Tile::ground = ground;
}

void Tile::addGround(float value) {
    ground += value;
}

void Tile::addHeat(float value) {
    heat += value;
}

void Tile::addHeight(float value) {
    height += value;
}

Tile::Tile(const Point &position) : position(position), heat(0), ground(0), height(0) {}

const Point &Tile::getPosition() const {
    return position;
}
