//
// Created by Amalric Lombard de Buffières on 8/9/20.
//

#include <string>
#include "HeatEnergyMinimap.h"
#include "../colors/RGBColor.h"

HeatEnergyMinimap::HeatEnergyMinimap(double pixelSize, double positionX, double positionY)
        : Minimap(pixelSize, positionX, positionY) {

}

std::string HeatEnergyMinimap::getName() {
    return "Heat";
}

void HeatEnergyMinimap::setPixelColor(int tileX, int tileY, Farm *farm) {
    float height = values[tileX][tileY];

    RGBColor rectangleColor = RGBColor(0.f, 1.f, height);

    sf::Color pixelColor = sf::Color(rectangleColor.getRed(), rectangleColor.getGreen(), rectangleColor.getBlue(), 255);

    int vertexArrayIndex = (tileX * TILE_COUNT_HEIGHT) + tileY;

    vertexArray[(vertexArrayIndex * 4) + 0].color = pixelColor;
    vertexArray[(vertexArrayIndex * 4) + 1].color = pixelColor;
    vertexArray[(vertexArrayIndex * 4) + 2].color = pixelColor;
    vertexArray[(vertexArrayIndex * 4) + 3].color = pixelColor;
}

void HeatEnergyMinimap::draw(sf::RenderWindow *window) {
    window->draw(vertexArray);
}


void HeatEnergyMinimap::generateValues(Farm * farm) {
    for (int it = 0; it < TILE_COUNT_WIDTH; it++) {
        for (int jt = 0; jt < TILE_COUNT_HEIGHT; jt++) {
            values[it][jt] = farm->getMap()->getHeatAt(it, jt) / 100.f;
        }
    }
}
