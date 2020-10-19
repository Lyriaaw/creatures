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
    std::lock_guard<std::mutex> guard(heatMutex);

    Tile::heat = heat;
}

float Tile::getGround() const {
    return ground;
}

void Tile::setGround(float ground) {
    std::lock_guard<std::mutex> guard(groundMutex);

    Tile::ground = ground;
}

void Tile::removeGround(float groundToRemove) {
    std::lock_guard<std::mutex> guard(groundMutex);
    Tile::ground -= groundToRemove;
}


void Tile::addGround(float value) {
    std::lock_guard<std::mutex> guard(groundMutex);
    ground += value;
}

void Tile::addHeat(float value) {
    std::lock_guard<std::mutex> guard(heatMutex);
    heat += value;
}

void Tile::addHeight(float value) {
    height += value;
}

Tile::Tile(float height, float color, float heat, float ground) : height(height), color(color), heat(heat), ground(ground) {}

float Tile::getColor() const {
    return color;
}

void Tile::setColor(float color) {
    Tile::color = color;
}
