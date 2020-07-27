//
// Created by Amalric Lombard de Buffières on 7/17/20.
//

#include "Farm.h"

#include <utility>
#include <iostream>


using namespace std;

Farm::Farm() {
}

void Farm::InitRandomMap() {
    map = Map();
    for (int it = 0; it < CHUNK_COUNT_WIDTH; it++) {
        for (int jt = 0; jt < CHUNK_COUNT_HEIGHT; jt++) {
            map.setTileAt(it, jt, (rand() % 21) - 10);
        }
    }
}

void Farm::InitFromRandom() {
    random_device rd;
    mt19937 mt(rd());
    uniform_real_distribution<double> distWidth(0, FARM_WIDTH);
    uniform_real_distribution<double> distHeight(0, FARM_HEIGHT);

    InitRandomMap();

    std::uniform_real_distribution<float> distMovement(-1, 1);
    for (int it = 0; it < 100; it++) {
        int x = distWidth(mt);
        int y = distHeight(mt);

        Point point(x, y);

        Entity entity(point, 10);
        entities.push_back(entity);

    }

}


void Farm::Tick() {
    for (int it = 0; it < entities.size(); it++) {
        entities.at(it).move();
    }
}


const vector<Entity> &Farm::getEntities() const {
    return entities;
}

void Farm::setEntities(const vector<Entity> &entities) {
    Farm::entities = entities;
}

void Farm::setUi(FarmUI ui) {
    Farm::ui = ui;
}

Map Farm::getMap() {
    return map;
}

const FarmUI &Farm::getUi() const {
    return ui;
}

