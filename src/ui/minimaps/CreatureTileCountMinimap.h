//
// Created by Amalric Lombard de Buffières on 8/8/20.
//

#ifndef CREATURES_CREATURETILECOUNTMINIMAP_H
#define CREATURES_CREATURETILECOUNTMINIMAP_H


#include "../../farm/Farm.h"
#include "Minimap.h"

class CreatureTileCountMinimap: public Minimap {
private:

    double values[TILE_COUNT_WIDTH][TILE_COUNT_HEIGHT];

public:
    CreatureTileCountMinimap(double pixelSize, double positionX, double positionY);
    std::string getName() override;

    void generateValues(Farm * farm);

    void setPixelColor(int tileX, int tileY, Farm * farm) override;
    void draw(sf::RenderWindow *window) override;
};


#endif //CREATURES_CREATURETILECOUNTMINIMAP_H
