//
// Created by Amalric Lombard de Buffières on 7/17/20.
//

#include "Map.h"
float Map::getTileAt(int chunkX, int chunkY) {
    return tiles[chunkX][chunkY];
}


float Map::setTileAt(int chunkX, int chunkY, float value) {
    tiles[chunkX][chunkY] = value;
}

