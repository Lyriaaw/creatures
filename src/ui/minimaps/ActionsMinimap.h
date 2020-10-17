//
// Created by Amalric Lombard de Buffières on 10/17/20.
//

#ifndef CREATURES_ACTIONSMINIMAP_H
#define CREATURES_ACTIONSMINIMAP_H


#include "Minimap.h"

class ActionsMinimap : public Minimap {

private:

    double r[TILE_COUNT_WIDTH][TILE_COUNT_HEIGHT];
    double g[TILE_COUNT_WIDTH][TILE_COUNT_HEIGHT];
    double b[TILE_COUNT_WIDTH][TILE_COUNT_HEIGHT];

public:
    ActionsMinimap();

    ActionsMinimap(double pixelSize, double positionX, double positionY);
    std::string getName() override;

    void generateValues(Farm * farm);

    void setPixelColor(int tileX, int tileY, Farm * farm) override;
    void draw(sf::RenderWindow *window) override;

    sf::Color getColorFromActionName(ActionDTO action);

    void processColor(double **grid);

    void processRed();

    void processGreen();

    void processBlue();
};


#endif //CREATURES_ACTIONSMINIMAP_H
