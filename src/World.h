//
// Created by Amalric Lombard de Buffières on 7/17/20.
//

#ifndef CREATURES_WORLD_H
#define CREATURES_WORLD_H

//#define CHUNK_SIZE TILE_SIZE * 6

#define TILE_SIZE 30
#define TILE_PER_CHUNK 10

#define CHUNK_SIZE TILE_SIZE * TILE_PER_CHUNK

#define CHUNK_COUNT_WIDTH 16
#define CHUNK_COUNT_HEIGHT 9

#define FARM_WIDTH CHUNK_COUNT_WIDTH * CHUNK_SIZE
#define FARM_HEIGHT CHUNK_COUNT_HEIGHT * CHUNK_SIZE

#define TILE_COUNT_WIDTH CHUNK_COUNT_WIDTH * TILE_PER_CHUNK
#define TILE_COUNT_HEIGHT CHUNK_COUNT_HEIGHT * TILE_PER_CHUNK

#define MUTATION_RATIO 1000

#define INITIAL_CREATURE_COUNT 1000
#define INITIAL_FOOD_COUNT 10000


#endif //CREATURES_WORLD_H
