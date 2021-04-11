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

    float currentValue = tmp_heat;
    tmp_heat = 0;

    return currentValue;
}

void Tile::addTmpGround(float value) {
    std::lock_guard<std::mutex> guard(tmp_ground_mutex);
    tmp_ground += value;
}
float Tile::getTmpGround() const {
    return tmp_ground;
}
float Tile::getAndClearTmpGround() {
    std::lock_guard<std::mutex> guard(tmp_ground_mutex);

    float currentValue = tmp_ground;
    tmp_ground = 0;

    return currentValue;
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

Tile::Tile(Point coordinates, float height, float heat, float ground) : coordinates(coordinates), height(height), heat(heat), ground(ground), pheromoneColor(0), pheromoneQuantity(0), tmp_heat(0), tmp_ground(0) {}

float Tile::getPheromoneColor() const {
    return pheromoneColor;
}

float Tile::getPheromoneQuantity() const {
    return pheromoneQuantity;
}

void Tile::addPheromone(float newColor, float newQuantity) {
    std::lock_guard<std::mutex> guard(pheromone_mutex);

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

    pheromoneQuantity -= (0.025 * pheromoneQuantity) * VEGETALISATION_RATE;

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

void Tile::addLife(Entity * life) {
    std::lock_guard<std::mutex> guard(lifes_mutex);
    lifes.emplace_back(life);
}

void Tile::removeLife(Entity * life) {
    std::lock_guard<std::mutex> guard(lifes_mutex);

    std::vector<Entity *> newLifes;
    for (int it = 0; it < lifes.size(); it++) {

        if (lifes.at(it)->getId() != life->getId()) {
            newLifes.emplace_back(lifes.at(it));
        }
    }

    lifes.clear();
    lifes = newLifes;
}





std::vector<Entity *> Tile::getEntities() {
    std::lock_guard<std::mutex> guard(entities_mutex);
    std::vector<Entity *> givenEntities = entities;
    return givenEntities;
}

std::vector<Entity *> Tile::getLifesEntities() {
    std::lock_guard<std::mutex> guard(lifes_mutex);


    std::vector<Entity *> givenLifes = lifes;
    return givenLifes;
}





std::vector<Entity *> Tile::lockOwnerGetEntities() {
    return entities;
}



void Tile::handleEntityDecay() {
    std::lock_guard<std::mutex> guard(entities_mutex);
    for (auto const& entity: entities) {
        if (!entity->tryLockInteraction()) {
//            std::cout << "Unable to perform decay on entity " << entity->getId() << std::endl;
            continue;
        }

        if (entity->getMass() < 100) {
            addGround(entity->getMass());
            entity->setMass(0);
        } else {
            addGround(0.5 * VEGETALISATION_RATE);
            entity->removeMass(0.5 * VEGETALISATION_RATE);
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

void Tile::lockHeat() {
    while (!heat_mutex.try_lock()) {
        usleep(10000);
    }
}

void Tile::unlockHeat() {
    heat_mutex.unlock();
}




void Tile::lockEntities() {
    while (!entities_mutex.try_lock()) {
        usleep(10000);
    }
}

void Tile::unlockEntities() {
    entities_mutex.unlock();
}

void Tile::lockLifes() {
    while (!lifes_mutex.try_lock()) {
        usleep(10000);
    }
}

void Tile::unlockLifes() {
    lifes_mutex.unlock();
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

void Tile::lockTmpHeatAndTmpGround() {
    while (!tmp_ground_mutex.try_lock()) {
        usleep(10000);
    }
    while (!tmp_heat_mutex.try_lock()) {
        usleep(10000);
    }
}

void Tile::unlockTmpHeatAndTmpGround() {
    tmp_ground_mutex.unlock();
    tmp_heat_mutex.unlock();
}

nlohmann::json Tile::asJSON() {
    nlohmann::json tileData = {
            {"height", height}
    };

    return tileData;
}
