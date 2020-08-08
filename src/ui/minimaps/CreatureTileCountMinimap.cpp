//
// Created by Amalric Lombard de Buffières on 8/8/20.
//

#include <string>
#include <SFML/Graphics.hpp>
#include "CreatureTileCountMinimap.h"
#include "../colors/RGBColor.h"

CreatureTileCountMinimap::CreatureTileCountMinimap(double pixelSize, double positionX, double positionY)
        : Minimap(pixelSize, positionX, positionY) {

}

std::string CreatureTileCountMinimap::getName() {
    return "Creature count";
}

void CreatureTileCountMinimap::setPixelColor(int tileX, int tileY, Farm *farm) {
    float hue = averageHues[tileX][tileY];
    float light = values[tileX][tileY] / 5.f;

    RGBColor rectangleColor = RGBColor(hue, 1.f, light);

    sf::Color pixelColor = sf::Color(rectangleColor.getRed(), rectangleColor.getGreen(), rectangleColor.getBlue(), 255);

    int vertexArrayIndex = (tileX * TILE_COUNT_HEIGHT) + tileY;

    vertexArray[(vertexArrayIndex * 4) + 0].color = pixelColor;
    vertexArray[(vertexArrayIndex * 4) + 1].color = pixelColor;
    vertexArray[(vertexArrayIndex * 4) + 2].color = pixelColor;
    vertexArray[(vertexArrayIndex * 4) + 3].color = pixelColor;
}

void CreatureTileCountMinimap::draw(sf::RenderWindow *window) {
    window->draw(vertexArray);
}


void CreatureTileCountMinimap::generateValues(Farm * farm) {
    for (int it = 0; it < TILE_COUNT_WIDTH; it++) {
        for (int jt = 0; jt < TILE_COUNT_HEIGHT; jt++) {
            averageHues[it][jt] = 0;
            values[it][jt] = 0;
        }
    }

    std::vector<BrainConnector *> currentConnectors = farm->getConnectors();

    for (int it = 0; it < currentConnectors.size(); it++) {
        Point point = currentConnectors.at(it)->getCreature()->getPosition();
        Point tilePosition = point.getTileCoordinates();

        values[int(tilePosition.getX())][int(tilePosition.getY())]++;
        averageHues[int(tilePosition.getX())][int(tilePosition.getY())] += currentConnectors.at(it)->getCreature()->getColor();
    }

    for (int it = 0; it < TILE_COUNT_WIDTH; it++) {
        for (int jt = 0; jt < TILE_COUNT_HEIGHT; jt++) {
            averageHues[it][jt] /= float(values[it][jt]);
        }
    }
}
