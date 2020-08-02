//
// Created by Amalric Lombard de Buffières on 7/17/20.
//

#include "Farm.h"
#include "perlin/PerlinNoise.h"

#include <iostream>


using namespace std;

Farm::Farm() {
}

void Farm::InitRandomMap() {
    float coolSeeds[] = {3041, 7980, 4672, 2354, 518, 6237, 868, 3815, 2727, 1568, 5953, 8058, 568654, 787145};
    float seed = rand() % 1000000;
//    float seed = 1568;
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

    for (int it = 0; it < 10000; it++) {
        int x = distWidth(mt);
        int y = distHeight(mt);

        Point point(x, y);


        Food * entity = new Food(point, 5);
        foods.push_back(entity);
    }
}


void Farm::Tick() {
    for (int it = 0; it < creatures.size(); it++) {
        creatures.at(it)->move();
    }

    generateEntityGrid();
}

void Farm::generateEntityGrid() {

    std::vector<std::vector<std::vector<Entity *>>> newEntityGrid;
    for (int it = 0; it < CHUNK_COUNT_WIDTH; it++) {
        std::vector<std::vector<Entity *>> line;
        for (int jt = 0; jt < CHUNK_COUNT_HEIGHT; jt++) {
            std::vector<Entity *> currentChunk;
            line.push_back(currentChunk);
        }
        newEntityGrid.push_back(line);
    }

    for (int it = 0; it < creatures.size(); it++) {
        Point simpleCoordinates = creatures.at(it)->getSimpleCoordinates();
        newEntityGrid.at(simpleCoordinates.getX()).at(simpleCoordinates.getY()).push_back(creatures.at(it));
    }

    for (int it = 0; it < foods.size(); it++) {
        Point simpleCoordinates = foods.at(it)->getSimpleCoordinates();
        newEntityGrid.at(simpleCoordinates.getX()).at(simpleCoordinates.getY()).push_back(foods.at(it));
    }

    entityGrid = newEntityGrid;
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

const vector<Food *> &Farm::getFoods() const {
    return foods;
}

