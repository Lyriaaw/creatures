//
// Created by Amalric Lombard de Buffières on 8/9/20.
//

#include <algorithm>
#include <iostream>
#include <unistd.h>
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

void Tile::lockOwnerSetHeat(float heat) {
    Tile::heat = heat;
}



float Tile::getGround() const {
    return ground;
}

void Tile::setGround(float ground) {
    std::lock_guard<std::mutex> guard(ground_mutex);
    Tile::ground = ground;
}

void Tile::lockOwnerSetGround(float ground) {
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

void Tile::lockOwnerAddGround(float value) {
    ground += value;
}



void Tile::addHeat(float value) {
    std::lock_guard<std::mutex> guard(heat_mutex);
    heat += value;
}

void Tile::addTmpHeat(float value) {
    std::lock_guard<std::mutex> guard(tmp_heat_mutex);
    tmp_heat += value;
}
float Tile::getTmpHeat() const {
    return tmp_heat;
}
float Tile::getAndClearTmpHeat() {
    std::lock_guard<std::mutex> guard(tmp_heat_mutex);

    double currentValue = tmp_heat;
    tmp_heat = 0;

    return tmp_heat;
}

void Tile::setTmoHeat(float heat) {
    std::lock_guard<std::mutex> guard(tmp_heat_mutex);
    Tile::tmp_heat = heat;
}




void Tile::lockOwnerAddHeat(float value) {
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
            lockOwnerAddGround(entity->getMass());
            entity->setMass(0);
        } else {
            lockOwnerAddGround(2.0);
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


void Tile::lockHeatAndGround() {
    while (!ground_mutex.try_lock()) {
        usleep(10000);
    }
    while (!heat_mutex.try_lock()) {
        usleep(10000);
    }

}

void Tile::unlockHeatAndGround() {
    ground_mutex.unlock();
    heat_mutex.unlock();
}



void Tile::lockGround() {
    while (!ground_mutex.try_lock()) {
        usleep(10000);
    }
}

void Tile::unlockGround() {
    ground_mutex.unlock();
}