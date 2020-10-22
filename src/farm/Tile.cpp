//
// Created by Amalric Lombard de Buffières on 8/9/20.
//

#include <algorithm>
#include <iostream>
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
    std::lock_guard<std::mutex> guard(heat_mutex);
    Tile::heat = heat;
}

float Tile::getGround() const {
    return ground;
}

void Tile::setGround(float ground) {
    std::lock_guard<std::mutex> guard(ground_mutex);

    Tile::ground = ground;
}

void Tile::removeGround(float groundToRemove) {
    std::lock_guard<std::mutex> guard(ground_mutex);

    Tile::ground -= groundToRemove;
}


void Tile::addGround(float value) {
    std::lock_guard<std::mutex> guard(ground_mutex);

    ground += value;
}

void Tile::addHeat(float value) {
    std::lock_guard<std::mutex> guard(heat_mutex);

    heat += value;
}

void Tile::addHeight(float value) {
    height += value;
}

Tile::Tile(Point coordinates, float height, float color, float heat, float ground) : coordinates(coordinates), height(height), color(color), heat(heat), ground(ground), pheromoneColor(0), pheromoneQuantity(0) {}

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
    if (pheromoneQuantity == 0) {
        return;
    }

    pheromoneQuantity -= 0.05 * pheromoneQuantity;

    if (pheromoneQuantity < 1.0) {
        pheromoneQuantity = 0;
        pheromoneColor = 0;
    }
}

const Point &Tile::getCoordinates() const {
    return coordinates;
}

void Tile::addEntity(Entity * entity) {
    std::lock_guard<std::mutex> guard(entities_mutex);
    entities.emplace_back(entity);
}

std::vector<Entity *> Tile::getEntities() {
    return entities;
}

void Tile::handleEntityDecay() {
    for (auto const& entity: entities) {
        entity->tryLockInteraction();

        if (entity->getMass() < 100) {
            addGround(entity->getMass());
            entity->setMass(0);
        } else {
            addGround(2.0);
            entity->removeMass(2.0);
        }
        entity->unlockInteraction();
    }
}

void Tile::removeDeletedEntities() {
    std::lock_guard<std::mutex> guard(entities_mutex);

    std::vector<Entity *> newEntities;
    for (int it = 0; it < entities.size(); it++) {

        if (entities.at(it)->isExists()) {
            newEntities.emplace_back(entities.at(it));
        }
    }

    entities.clear();
    entities = newEntities;
}

