//
// Created by Amalric Lombard de Buffières on 8/8/20.
//

#include "Minimap.h"
#include "../colors/RGBColor.h"


Minimap::Minimap(double pixelSize, double positionX, double positionY) :pixelSize(pixelSize), positionX(
                                                                                             positionX),
                                                                                     positionY(
                                                                                             positionY){
    load();
}

void Minimap::load() {
    vertexArray = sf::VertexArray(sf::Quads, TILE_COUNT_WIDTH * TILE_COUNT_HEIGHT * 4);

    for (int it = 0; it < TILE_COUNT_WIDTH; it++) {
        for (int jt = 0; jt < TILE_COUNT_HEIGHT; jt++) {

            int vertexArrayIndex = (it * TILE_COUNT_HEIGHT) + jt;


            vertexArray[(vertexArrayIndex * 4) + 0].position = sf::Vector2f(
                    (float(it) * pixelSize) + positionX,
                    (float(jt) * pixelSize) + positionY);


            vertexArray[(vertexArrayIndex * 4) + 1].position = sf::Vector2f(
                    (float(it) * pixelSize) + positionX + pixelSize,
                    (float(jt) * pixelSize) + positionY);


            vertexArray[(vertexArrayIndex * 4) + 2].position = sf::Vector2f(
                    (float(it) * pixelSize) + positionX + pixelSize,
                    (float(jt) * pixelSize) + positionY + pixelSize);


            vertexArray[(vertexArrayIndex * 4) + 3].position = sf::Vector2f(
                    (float(it) * pixelSize) + positionX,
                    (float(jt) * pixelSize) + positionY + pixelSize);

        }
    }
}


void Minimap::move(double positionX, double positionY, double width, double height) {
    for (int it = 0; it < TILE_COUNT_WIDTH; it++) {
        for (int jt = 0; jt < TILE_COUNT_HEIGHT; jt++) {

            int vertexArrayIndex = (it * TILE_COUNT_HEIGHT) + jt;


            vertexArray[vertexArrayIndex + 0].position = sf::Vector2f(
                    (float(it) * pixelSize) + positionX,
                    (float(jt) * pixelSize) + positionY);


            vertexArray[vertexArrayIndex + 1].position = sf::Vector2f(
                    (float(it) * pixelSize) + positionX + pixelSize,
                    (float(jt) * pixelSize) + positionY);


            vertexArray[vertexArrayIndex + 2].position = sf::Vector2f(
                    (float(it) * pixelSize) + positionX + pixelSize,
                    (float(jt) * pixelSize) + positionY + pixelSize);


            vertexArray[vertexArrayIndex + 3].position = sf::Vector2f(
                    (float(it) * pixelSize) + positionX,
                    (float(jt) * pixelSize) + positionY + pixelSize);
        }
    }
}


void Minimap::changeSize(double width, double height) {
    move(positionX, positionY, width, height);
}






// World map
WorldMinimap::WorldMinimap(double pixelSize, double positionX, double positionY)
        : Minimap(pixelSize, positionX, positionY) {

}

std::string WorldMinimap::getName() {
    return "World";
}

void WorldMinimap::setPixelColor(int tileX, int tileY, Farm *farm) {
    float height = farm->getMap()->getTileAt(tileX, tileY);
    RGBColor rectangleColor = RGBColor(0.f, 0.f, ((height + 1) / 2));
    if (height < -0.1f) {
        rectangleColor = RGBColor(0.6f, 1.f, ((height + 1) / 2));
    }
    if (height > 0.7f) {
        rectangleColor = RGBColor(0.59f, 1.f, ((height + 1) / 2));
    }

    sf::Color pixelColor = sf::Color(rectangleColor.getRed(), rectangleColor.getGreen(), rectangleColor.getBlue(), 255);


    int vertexArrayIndex = (tileX * TILE_COUNT_HEIGHT) + tileY;

    vertexArray[(vertexArrayIndex * 4) + 0].color = pixelColor;
    vertexArray[(vertexArrayIndex * 4) + 1].color = pixelColor;
    vertexArray[(vertexArrayIndex * 4) + 2].color = pixelColor;
    vertexArray[(vertexArrayIndex * 4) + 3].color = pixelColor;
}



void WorldMinimap::draw(sf::RenderWindow *window) {
    window->draw(vertexArray);
}



