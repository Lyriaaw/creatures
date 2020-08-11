//
// Created by Amalric Lombard de Buffières on 8/9/20.
//

#ifndef CREATURES_HEATENERGYMINIMAP_H
#define CREATURES_HEATENERGYMINIMAP_H


#include "Minimap.h"


class HeatEnergyMinimap: public Minimap {
private:

    double values[TILE_COUNT_WIDTH][TILE_COUNT_HEIGHT];

public:
    HeatEnergyMinimap(double pixelSize, double positionX, double positionY);

    HeatEnergyMinimap();

    std::string getName() override;

    void generateValues(Farm * farm);

    void setPixelColor(int tileX, int tileY, Farm * farm) override;
    void draw(sf::RenderWindow *window) override;
};


#endif //CREATURES_HEATENERGYMINIMAP_H
