//
// Created by Amalric Lombard de Buffières on 8/9/20.
//

#include <algorithm>
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

void Tile::removeGround(float groundToRemove) {
    Tile::ground -= groundToRemove;
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

Tile::Tile(float height, float color, float heat, float ground) : height(height), color(color), heat(heat), ground(ground), pheromoneColor(0), pheromoneQuantity(0) {}

float Tile::getColor() const {
    return color;
}

void Tile::setColor(float color) {
    Tile::color = color;
}

float Tile::getPheromoneColor() const {
    return pheromoneColor;
}

float Tile::getPheromoneQuantity() const {
    return pheromoneQuantity;
}

void Tile::addPheromone(float newColor, float newQuantity) {
    pheromoneColor = ((pheromoneColor * pheromoneQuantity) + (newColor * newQuantity)) / (pheromoneQuantity + newQuantity);

    pheromoneQuantity += newQuantity;
}

void Tile::removePhermoneQuantity(float quantityRemoved) {
    pheromoneQuantity -= std::min(quantityRemoved, pheromoneQuantity);
}
void Tile::decayPheromone() {
    pheromoneQuantity -= 0.05 * pheromoneQuantity;

    if (pheromoneQuantity < 1.0) {
        pheromoneQuantity = 0;
        pheromoneColor = 0;
    }
}
