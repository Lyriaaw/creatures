//
// Created by Amalric Lombard de Buffières on 8/17/20.
//

#ifndef CREATURES_CHUNKSTEPMINIMAP_H
#define CREATURES_CHUNKSTEPMINIMAP_H


#include "Minimap.h"

class ChunkStepMinimap: public Minimap {
private:
    double values[TILE_COUNT_WIDTH][TILE_COUNT_HEIGHT];

    std::vector<std::string> steps;
    std::vector<sf::Color> colors;

    std::vector<sf::Text> legends;

public:
    ChunkStepMinimap(sf::Font * font);

    void setPixelColor(int tileX, int tileY, Farm *farm) override;

    void draw(sf::RenderWindow *window) override;

    std::string getName() override;
};


#endif //CREATURES_CHUNKSTEPMINIMAP_H
