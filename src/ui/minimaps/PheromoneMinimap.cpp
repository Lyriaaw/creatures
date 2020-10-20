//
// Created by Amalric Lombard de Buffières on 10/20/20.
//

#include "PheromoneMinimap.h"
#include "../colors/RGBColor.h"

PheromoneMinimap::PheromoneMinimap(double pixelSize, double positionX, double positionY)
        : Minimap(pixelSize, positionX, positionY) {

}

std::string PheromoneMinimap::getName() {
    return "Pheromone";
}

void PheromoneMinimap::setPixelColor(int tileX, int tileY, Farm *farm) {
    int vertexArrayIndex = (tileX * TILE_COUNT_HEIGHT) + tileY;

    vertexArray[(vertexArrayIndex * 4) + 0].color = colorGrid[tileX][tileY];
    vertexArray[(vertexArrayIndex * 4) + 1].color = colorGrid[tileX][tileY];
    vertexArray[(vertexArrayIndex * 4) + 2].color = colorGrid[tileX][tileY];
    vertexArray[(vertexArrayIndex * 4) + 3].color = colorGrid[tileX][tileY];
}



void PheromoneMinimap::draw(sf::RenderWindow *window) {
    window->draw(vertexArray);
}

PheromoneMinimap::PheromoneMinimap(): Minimap(){}

void PheromoneMinimap::generateValues(Farm *farm) {
    for (int it = 0; it < TILE_COUNT_WIDTH; it++) {
        for (int jt = 0; jt < TILE_COUNT_HEIGHT; jt++) {
            Tile * tile = farm->getMap()->getTileAt(it, jt);
            RGBColor rectangleColor = RGBColor(tile->getPheromoneColor(), 1.f, 0.5);

            sf::Color pixelColor = sf::Color(rectangleColor.getRed(), rectangleColor.getGreen(), rectangleColor.getBlue(), (tile->getPheromoneQuantity() / 500.0) * 255);

            colorGrid[it][jt] = pixelColor;
        }
    }
}