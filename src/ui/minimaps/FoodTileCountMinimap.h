//
// Created by Amalric Lombard de Buffières on 8/8/20.
//

#ifndef CREATURES_FOODTILECOUNTMINIMAP_H
#define CREATURES_FOODTILECOUNTMINIMAP_H


#include "Minimap.h"

class FoodTileCountMinimap: public Minimap {
private:

public:
    FoodTileCountMinimap();

private:

    double averageHues[TILE_COUNT_WIDTH][TILE_COUNT_HEIGHT];
    double values[TILE_COUNT_WIDTH][TILE_COUNT_HEIGHT];

public:
    FoodTileCountMinimap(double pixelSize, double positionX, double positionY);
    std::string getName() override;

    void generateValues(Farm * farm);

    void setPixelColor(int tileX, int tileY, Farm * farm) override;
    void draw(sf::RenderWindow *window) override;
};


#endif //CREATURES_FOODTILECOUNTMINIMAP_H
