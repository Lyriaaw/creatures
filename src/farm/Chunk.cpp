//
// Created by Amalric Lombard de Buffières on 8/15/20.
//

#include <zconf.h>
#include "Chunk.h"
#include "../utils/perlin/PerlinNoise.h"

Chunk::Chunk(Point chunkPosition): chunkPosition(chunkPosition), step("init") {
    generateNeighbours();
}

void Chunk::generateNeighbours() {
    for (int it = -1; it <= 1; it++) {
        std::vector<Chunk *> neighbourgsLine;
        for (int jt = -1; jt <= 1; jt++) {
            neighbourgsLine.emplace_back(nullptr);
        }
        neighbours.emplace_back(neighbourgsLine);
    }
}

void Chunk::generateRandomChunk(int seed, float min, float max) {
    PerlinNoise perlin(seed);

    float removed = ((max - min) / 2.f) + min;
    float ratio = (1.f / (max - min)) * 2.f;

    int deltaX = chunkPosition.getX() * TILE_PER_CHUNK;
    int deltaY = chunkPosition.getY() * TILE_PER_CHUNK;

    for (int it = 0; it < TILE_PER_CHUNK; it++) {
        std::vector<Tile *> line;
        for (int jt = 0; jt < TILE_PER_CHUNK; jt++) {

            float xComponent = (float(it + deltaX) / float(TILE_COUNT_WIDTH)) * 2.5;
            float yComponent = (float(jt + deltaY) / float(TILE_COUNT_HEIGHT)) * 2.5;

            float height = perlin.noise(xComponent, yComponent, 0.8);

            height -= removed;
            height *= ratio;
            height *= height * height;

            Tile * currentTile = new Tile(Point(it + deltaX, jt + deltaY));
            currentTile->setHeight(height);
            currentTile->setGround(1500.0);

            line.emplace_back(currentTile);

            currentTile->setHeight(height);

        }
        tiles.emplace_back(line);
    }
}

Tile * Chunk::getTileAt(int tileX, int tileY) {
    if (tileX < 0 || tileX >= TILE_COUNT_WIDTH || tileY < 0 || tileY >= TILE_COUNT_HEIGHT) {
        std::cout << "ERROR, REQUESTED WRONG TILE => X: " << tileX << " Y: " << tileY << std::endl;
        return tiles.at(0).at(0);
    }

    Point requestedChunkPosition = Point(tileX / TILE_PER_CHUNK, tileY / TILE_PER_CHUNK);

    if (!requestedChunkPosition.equals(chunkPosition)) {
        std::cout << "ERROR, REQUESTED TILE IN WRONG CHUNK: X: " << tileX << " Y: " << tileY << " | ";
        std::cout << "CURRENT CHUNK => X: " << chunkPosition.getX() << " Y: " << chunkPosition.getY() << std::endl;
        return tiles.at(0).at(0);
    }

    int deltaX = chunkPosition.getX() * TILE_PER_CHUNK;
    int deltaY = chunkPosition.getY() * TILE_PER_CHUNK;

    Tile * tile = tiles.at(tileX - deltaX).at(tileY - deltaY);

    return tile;
}

void Chunk::setNeighbour(int it, int jt, Chunk * neighbour) {
    neighbours.at(it + 1).at(jt + 1) = neighbour;
}


void Chunk::waitForNeighbours(std::vector<std::string> requestedSteps) {

    bool allNeighboursReady(true);

    do {
        allNeighboursReady = true;

        for (int it = 0; it < 3; it++) {
            for (int jt = 0; jt < 3; jt++) {

                Chunk * neighbour = neighbours.at(it).at(jt);

                if (neighbour == nullptr || neighbour->getChunkPosition().equals(chunkPosition))
                    continue;

                bool found(false);
                for (int kt = 0; kt < requestedSteps.size(); kt++) {
                    if (neighbour->getStep() == requestedSteps.at(kt)) {
                        found = true;
                    }
                }

                if (!found) {
                    allNeighboursReady = false;
                }

            }
        }

        if (!allNeighboursReady) {
            usleep(100);
        }

    }while (!allNeighboursReady);
}

void Chunk::processClimate() {


    double newGround[TILE_PER_CHUNK][TILE_PER_CHUNK];
    double newHeats[TILE_PER_CHUNK][TILE_PER_CHUNK];

    this->step = "CLIMATE_START";
    std::vector<std::string> steps;
    steps.emplace_back("CLIMATE_READY");
    steps.emplace_back("CLIMATE_START");
    waitForNeighbours(steps);

    for (int it = 0; it < TILE_PER_CHUNK; it++) {
        for (int jt = 0; jt < TILE_PER_CHUNK; jt++) {
            Tile * currentTile = getRelativeTile(it, jt);
            currentTile->processAddedGround();
            currentTile->processAddedHeat();

            newGround[it][jt] = currentTile->getGround();
            newHeats[it][jt] = currentTile->getHeat();
        }
    }

    int deltaX = chunkPosition.getX() * TILE_PER_CHUNK;
    int deltaY = chunkPosition.getY() * TILE_PER_CHUNK;

    this->step = "CLIMATE_READY";
    steps.clear();
    steps.emplace_back("CLIMATE_READY");
    steps.emplace_back("CLIMATE_SPREAD");
    waitForNeighbours(steps);

    for (int it = 0; it < TILE_PER_CHUNK; it++) {
        for (int jt = 0; jt < TILE_PER_CHUNK; jt++) {
            Tile * currentTile = tiles.at(it).at(jt);
//            std::cout << "Processing climate on tile X: " << currentTile->getPosition().getX() << " Y: " << currentTile->getPosition().getY() << std::endl;

            double availableGround = currentTile->getGround() / 100.0;


            double currentHeight = currentTile->getHeight();
            double availableHeat = currentTile->getHeat() / 10.0;

            if (availableGround == 0 && availableHeat == 0) {
                continue;
            }

            for (int x = -1; x <= 1; x++) {
                for (int y = -1; y <= 1; y++) {
                    int totalX = it + x + deltaX;
                    int totalY = jt + y + deltaY;
                    if (it + x + deltaX < 0 || it + x + deltaX >= TILE_COUNT_WIDTH ||jt + y + deltaY < 0 || jt + y + deltaY >= TILE_COUNT_HEIGHT) {
                        continue;
                    }

                    Tile * relativeTile = getRelativeTile(it + x, jt + y);
//
//                    if (relativeTile->getPosition().equals(currentTile->getPosition())) {
//                        continue;
//                    }


                    // If the other tile is lower than this one, we add some heat to be transfer
                    // the lowest the target tile, the biggest is the added energy
                    double heightDifference = (currentHeight - relativeTile->getHeight()) / 5.f;

                    double transferedHeat = availableHeat + (heightDifference * availableHeat);
                    relativeTile->addHeat(transferedHeat);
                    newHeats[it][jt] -= transferedHeat;




                    double transferedGround = availableGround + (heightDifference * availableGround);
                    relativeTile->addGround(transferedGround);
                    newGround[it][jt] -= transferedGround;
                }
            }

        }
    }



    this->step = "CLIMATE_SPREAD";
    steps.clear();
    steps.emplace_back("CLIMATE_SPREAD");
    steps.emplace_back("CLIMATE_FINISHED");
    waitForNeighbours(steps);


//    for (int it = 0; it < TILE_PER_CHUNK; it++) {
//        for (int jt = 0; jt < TILE_PER_CHUNK; jt++) {
//            Tile * currentTile = getRelativeTile(it, jt);
//            currentTile->setGround(newGround[it][jt]);
//            currentTile->setHeat(newHeats[it][jt]);
//            currentTile->processAddedGround();
//            currentTile->processAddedHeat();
//        }
//    }




    for (int it = 0; it < TILE_PER_CHUNK; it++) {
        for (int jt = 0; jt < TILE_PER_CHUNK; jt++) {
            Tile * currentTile = getRelativeTile(it, jt);

            if (newGround[it][jt] < 0) {
                std::cout << "Climate to ground " << newGround[it][jt] << std::endl;
            }
            currentTile->setGround(newGround[it][jt]);
            currentTile->setHeat(newHeats[it][jt]);

            currentTile->processAddedHeat();
            currentTile->processAddedGround();

            if (currentTile->getHeat() <= 0) {
                continue;
            }

            float heatToGroundRatio = 0.01f;
            float currentTileHeat = currentTile->getHeat();

            currentTile->setHeat(currentTile->getHeat() - (currentTileHeat * heatToGroundRatio));
            currentTile->addGround(currentTileHeat * heatToGroundRatio);

            currentTile->processAddedGround();
        }
    }

    this->step = "CLIMATE_FINISHED";

}

Tile * Chunk::getRelativeTile(int tileX, int tileY) {

    if (tileX < 0 || tileX >= TILE_PER_CHUNK || tileY < 0 || tileY >= TILE_PER_CHUNK) {
        int ratioX = 0;
        if (tileX < 0)
            ratioX = -1;
        if (tileX >= TILE_PER_CHUNK)
            ratioX = 1;

        int ratioY = 0;
        if (tileY < 0)
            ratioY = -1;
        if (tileY >= TILE_PER_CHUNK)
            ratioY = 1;

        int deltaX = chunkPosition.getX() * TILE_PER_CHUNK;
        int deltaY = chunkPosition.getY() * TILE_PER_CHUNK;

        int requestedX = deltaX + tileX;
        int requestedY = deltaY + tileY;

        Tile * foundTile = neighbours.at(ratioX + 1).at(ratioY + 1)->getTileAt(requestedX, requestedY);

        return foundTile;
    }

    Tile * tile = tiles.at(tileX).at(tileY);
    return tile;
}

const std::string &Chunk::getStep() const {
    return step;
}

const Point &Chunk::getChunkPosition() const {
    return chunkPosition;
}




