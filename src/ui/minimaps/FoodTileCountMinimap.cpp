//
// Created by Amalric Lombard de Buffières on 8/8/20.
//

#include "FoodTileCountMinimap.h"
#include "../colors/RGBColor.h"

FoodTileCountMinimap::FoodTileCountMinimap(double pixelSize, double positionX, double positionY)
        : Minimap(pixelSize, positionX, positionY) {

}

std::string FoodTileCountMinimap::getName() {
    return "Creature count";
}

void FoodTileCountMinimap::setPixelColor(int tileX, int tileY, Farm *farm) {
    float height = values[tileX][tileY] / 5.f;

    RGBColor rectangleColor = RGBColor(0.28f, 1.f, height);

    sf::Color pixelColor = sf::Color(rectangleColor.getRed(), rectangleColor.getGreen(), rectangleColor.getBlue(), 255);

    int vertexArrayIndex = (tileX * TILE_COUNT_HEIGHT) + tileY;

    vertexArray[(vertexArrayIndex * 4) + 0].color = pixelColor;
    vertexArray[(vertexArrayIndex * 4) + 1].color = pixelColor;
    vertexArray[(vertexArrayIndex * 4) + 2].color = pixelColor;
    vertexArray[(vertexArrayIndex * 4) + 3].color = pixelColor;
}

void FoodTileCountMinimap::draw(sf::RenderWindow *window) {
    window->draw(vertexArray);
}


void FoodTileCountMinimap::generateValues(Farm * farm) {
    for (int it = 0; it < TILE_COUNT_WIDTH; it++) {
        for (int jt = 0; jt < TILE_COUNT_HEIGHT; jt++) {
            values[it][jt] = 0;
        }
    }

    std::vector<Entity *> currentEntities = farm->getEntities();

    for (int it = 0; it < currentEntities.size(); it++) {
        Point point = currentEntities.at(it)->getPosition();
        Point tilePosition = point.getTileCoordinates();

        values[int(tilePosition.getX())][int(tilePosition.getY())]++;
    }
}
