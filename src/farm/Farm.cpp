//
// Created by Amalric Lombard de Buffières on 7/17/20.
//

#include "Farm.h"



using namespace std;

Farm::Farm() {
}

void Farm::InitRandomMap() {
    for (int it = 0; it < CHUNK_COUNT_WIDTH; it++) {
        for (int jt = 0; jt < CHUNK_COUNT_HEIGHT; jt++) {
            map[it][jt] = (rand() % 21) - 10;
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

        Entity entity(point, 20);
        entities.push_back(entity);

    }

}


void Farm::Tick() {
    for (int it = 0; it < entities.size(); it++) {
        entities.at(it).move();
    }
}

float Farm::getMapAt(int chunkX, int chunkY) {
    return map[chunkX][chunkY];
}

const vector<Entity> &Farm::getEntities() const {
    return entities;
}

void Farm::setEntities(const vector<Entity> &entities) {
    Farm::entities = entities;
}

FarmUI *Farm::getUi() const {
    return ui;
}

void Farm::setUi(FarmUI *ui) {
    Farm::ui = ui;
}

