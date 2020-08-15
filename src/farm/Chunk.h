//
// Created by Amalric Lombard de Buffières on 8/15/20.
//

#ifndef CREATURES_CHUNK_H
#define CREATURES_CHUNK_H


#include <vector>
#include "life/Life.h"


class Chunk {
private:
    Point chunkPosition;


    std::vector<Life *> lifes;

    std::vector<Life *> creatures;
    std::vector<Life *> vegetals;

    std::vector<Entity *> entities;

    std::vector<std::vector<std::vector<Entity *>>> entityGrid;

    std::vector<ActionDTO> actions;

    std::vector<std::vector<Tile *>> tiles;
    std::vector<std::vector<Chunk *>> neighbours;

public:

    Chunk(Point chunkPosition);

    void generateRandomChunk(int seed, float min, float max);

    Tile *getTileAt(int tileX, int tileY);

    void generateNeighbours();

    void setNeighbour(int it, int jt, Chunk *neighbour);
};


#endif //CREATURES_CHUNK_H
