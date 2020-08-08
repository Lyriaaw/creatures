//
// Created by Amalric Lombard de Buffières on 8/9/20.
//

#include "GroundEnergyMinimap.h"
#include "../colors/RGBColor.h"

GroundEnergyMinimap::GroundEnergyMinimap(double pixelSize, double positionX, double positionY)
        : Minimap(pixelSize, positionX, positionY) {

}

std::string GroundEnergyMinimap::getName() {
    return "Heat";
}

void GroundEnergyMinimap::setPixelColor(int tileX, int tileY, Farm *farm) {
    float height = values[tileX][tileY] / 5.f;

    RGBColor rectangleColor = RGBColor(0.28f, 1.f, height);

    sf::Color pixelColor = sf::Color(rectangleColor.getRed(), rectangleColor.getGreen(), rectangleColor.getBlue(), 255);

    int vertexArrayIndex = (tileX * TILE_COUNT_HEIGHT) + tileY;

    vertexArray[(vertexArrayIndex * 4) + 0].color = pixelColor;
    vertexArray[(vertexArrayIndex * 4) + 1].color = pixelColor;
    vertexArray[(vertexArrayIndex * 4) + 2].color = pixelColor;
    vertexArray[(vertexArrayIndex * 4) + 3].color = pixelColor;
}

void GroundEnergyMinimap::draw(sf::RenderWindow *window) {
    window->draw(vertexArray);
}


void GroundEnergyMinimap::generateValues(Farm * farm) {
    for (int it = 0; it < TILE_COUNT_WIDTH; it++) {
        for (int jt = 0; jt < TILE_COUNT_HEIGHT; jt++) {
            values[it][jt] = farm->getMap()->getGroundAt(it, jt) / 100;
        }
    }
}
