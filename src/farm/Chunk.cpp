//
// Created by Amalric Lombard de Buffières on 8/15/20.
//

#include "Chunk.h"
#include "../utils/perlin/PerlinNoise.h"

Chunk::Chunk(Point chunkPosition): chunkPosition(chunkPosition) {
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

            Tile * currentTile = new Tile(Point(it, jt));
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