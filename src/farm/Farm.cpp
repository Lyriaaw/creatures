//
// Created by Amalric Lombard de Buffières on 7/17/20.
//

#include "Farm.h"
#include "Creature.h"
#include "perlin/PerlinNoise.h"

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


        Creature * entity = new Creature(point, 10);
        creatures.push_back(entity);
    }

}


void Farm::Tick() {
    for (int it = 0; it < creatures.size(); it++) {
        creatures.at(it)->move();
    }
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

const vector<Creature *> &Farm::getCreatures() const {
    return creatures;
}

void Farm::setCreatures(const vector<Creature *> &creatures) {
    Farm::creatures = creatures;
}

