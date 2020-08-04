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
    float coolSeeds[] = {3041, 7980, 4672, 2354, 518, 6237, 868, 3815, 2727, 1568, 5953, 8058, 568654, 787145, 924505, 117802, 523117, 45482, 407575, 391032, 660340, 526115, 218205, 890683, 595048, 236189};
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

    for (int it = 0; it < CHUNK_COUNT_WIDTH; it++) {
        std::vector<std::vector<Entity *>> line;
        for (int jt = 0; jt < CHUNK_COUNT_HEIGHT; jt++) {
            std::vector<Entity *> currentChunk;
            line.push_back(currentChunk);
        }
        entityGrid.push_back(line);
    }
}

void Farm::InitFromRandom() {
    random_device rd;
    mt19937 mt(rd());
    uniform_real_distribution<double> distWidth(0, FARM_WIDTH);
    uniform_real_distribution<double> distHeight(0, FARM_HEIGHT);

    InitRandomMap();

    std::uniform_real_distribution<float> distMovement(-1, 1);
    nursery = new CreatureNursery();
    for (int it = 0; it < 1000; it++) {
        connectors.push_back(nursery->generateFromRandom());
    }

    for (int it = 0; it < 10000; it++) {
        int x = distWidth(mt);
        int y = distHeight(mt);

        Point point(x, y);


        float foodSize = ((rand() % 500) / 100.f) + 1;

        Food * entity = new Food(point, foodSize);
        foods.push_back(entity);
    }
}


void Farm::Tick(bool paused) {
    averageSelectedEntities = 0;
    for (int it = 0; it < connectors.size(); it++) {
        Creature *currentCreature = connectors.at(it)->getCreature();


        if (!paused) {
            connectors.at(it)->processBrainOutputs();
        }

        if (!paused) {
            std::vector<Entity *> selectedEntities = getAccessibleEntities(currentCreature);
            currentCreature->executeAction(selectedEntities);
            currentCreature->move();

            averageSelectedEntities += selectedEntities.size();
        }
    }

    averageSelectedEntities /= float(connectors.size());




    generateEntityGrid();


    for (int it = 0; it < connectors.size(); it++) {
    Creature * currentCreature = connectors.at(it)->getCreature();


        currentCreature->findSelectedChunks();
        currentCreature->processSensorsValues(getAccessibleEntities(currentCreature));
        connectors.at(it)->processBrainInputs();
        connectors.at(it)->processBrain();



    }

}

std::vector<Entity *> Farm::getAccessibleEntities(Creature * creature) {
    std::vector<Entity *> accessibleEntities;
    for (int jt = 0; jt < creature->getSelectedChunks().size(); jt++) {
        Point currentChunk = creature->getSelectedChunks().at(jt);

        std::vector<Entity *> chunkEntities = entityGrid.at(currentChunk.getX()).at(currentChunk.getY());
        accessibleEntities.insert(accessibleEntities.end(), chunkEntities.begin(), chunkEntities.end());
    }
    return accessibleEntities;

}

void Farm::generateEntityGrid() {
    for (int it = 0; it < CHUNK_COUNT_WIDTH; it++) {
        for (int jt = 0; jt < CHUNK_COUNT_HEIGHT; jt++) {
            entityGrid.at(it).at(jt).clear();
        }
    }

    for (int it = 0; it < connectors.size(); it++) {
        Point simpleCoordinates = connectors.at(it)->getCreature()->getSimpleCoordinates();
        entityGrid.at(simpleCoordinates.getX()).at(simpleCoordinates.getY()).push_back(connectors.at(it)->getCreature());
    }

    for (int it = 0; it < foods.size(); it++) {
        Point simpleCoordinates = foods.at(it)->getSimpleCoordinates();
        entityGrid.at(simpleCoordinates.getX()).at(simpleCoordinates.getY()).push_back(foods.at(it));
    }

}





Map * Farm::getMap() {
    return &map;
}

const vector<Food *> &Farm::getFoods() const {
    return foods;
}

CreatureNursery *Farm::getNursery() const {
    return nursery;
}

const vector<BrainConnector *> &Farm::getConnectors() const {
    return connectors;
}

FarmUI *Farm::getUi() const {
    return ui;
}

void Farm::setUi(FarmUI *ui) {
    Farm::ui = ui;
}

void Farm::addConnector(BrainConnector * connector) {
    this->connectors.push_back(connector);
}

float Farm::getAverageSelectedEntities() const {
    return averageSelectedEntities;
}

