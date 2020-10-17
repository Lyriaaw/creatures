//
// Created by Amalric Lombard de Buffières on 8/8/20.
//

#ifndef CREATURES_MINIMAP_H
#define CREATURES_MINIMAP_H


#include <string>
#include <SFML/Graphics.hpp>
#include "../../farm/Farm.h"

class Minimap {
protected:
    double positionX, positionY, width, height;

    sf::VertexArray vertexArray;

    double pixelSize;

    sf::Color colorGrid[TILE_COUNT_WIDTH][TILE_COUNT_HEIGHT];


public:

    Minimap();
    Minimap(double pixelSize, double positionX, double positionY);
    void load();

    void move(double positionX, double positionY, double width, double height);
    void changeSize(double width, double height);

    virtual void setPixelColor(int tileX, int tileY, Farm * farm) = 0;
    virtual void draw(sf::RenderWindow *window) = 0;
    virtual void generateValues(Farm * farm) = 0;
    sf::Color getColorAt(int it, int jt);

    virtual std::string getName() = 0;

    double getPixelSize() const;

};


class WorldMinimap: public Minimap {
public:

    WorldMinimap();

    WorldMinimap(double pixelSize, double positionX, double positionY);
    std::string getName() override;

    void setPixelColor(int tileX, int tileY, Farm * farm) override;
    void draw(sf::RenderWindow *window) override;

    void generateValues(Farm *farm) override;


};





#endif //CREATURES_MINIMAP_H
