//
// Created by Amalric Lombard de Buffières on 8/9/20.
//

#ifndef CREATURES_GROUNDENERGYMINIMAP_H
#define CREATURES_GROUNDENERGYMINIMAP_H


#include "Minimap.h"

class GroundEnergyMinimap: public Minimap {
private:

    double colors[TILE_COUNT_WIDTH][TILE_COUNT_HEIGHT];
    double values[TILE_COUNT_WIDTH][TILE_COUNT_HEIGHT];

public:
    GroundEnergyMinimap(double pixelSize, double positionX, double positionY);

    GroundEnergyMinimap();

    std::string getName() override;

    void generateValues(Farm * farm);

    void setPixelColor(int tileX, int tileY, Farm * farm) override;
    void draw(sf::RenderWindow *window) override;
};


#endif //CREATURES_GROUNDENERGYMINIMAP_H
