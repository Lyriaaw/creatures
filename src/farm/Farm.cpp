//
// Created by Amalric Lombard de Buffières on 7/17/20.
//

#include "Farm.h"
#include "../PerlinNoise.h"

#include <utility>
#include <iostream>


using namespace std;

Farm::Farm() {
}

void Farm::InitRandomMap() {
//    float seed = rand() % 10000;
    float seed = 7980;
    PerlinNoise perlin(seed);

    cout << "Map generated with seed " << seed << endl;

    map = Map();
    for (int it = 0; it < CHUNK_COUNT_WIDTH; it++) {
        for (int jt = 0; jt < CHUNK_COUNT_HEIGHT; jt++) {
            float height = perlin.noise(float(it) / float(CHUNK_COUNT_WIDTH) * 2.5, float(jt) / float(CHUNK_COUNT_HEIGHT) * 2.5, 0.8) - 0.4f;


            map.setTileAt(it, jt, height);
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
    for (int it = 0; it < 1000; it++) {
        int x = distWidth(mt);
        int y = distHeight(mt);

        Point point(x, y);


        Entity * entity = new Entity(point, 10);
        entities.push_back(entity);
    }

}


void Farm::Tick() {
    for (int it = 0; it < entities.size(); it++) {
        entities.at(it)->move();
    }
}


void Farm::setEntities(const vector<Entity *> entities) {
    Farm::entities = entities;
}

std::vector<Entity *> Farm::getEntities() const {
    return entities;
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

