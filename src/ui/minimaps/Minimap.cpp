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
    for (int it = 0; it < TILE_COUNT_WIDTH; it++) {
        std::vector<sf::RectangleShape> line;
        for (int jt = 0; jt < TILE_COUNT_HEIGHT; jt++) {

            sf::RectangleShape rectangle = sf::RectangleShape(sf::Vector2f(pixelSize, pixelSize));

            rectangle.setPosition((float(it) * pixelSize) + positionX, (float(jt) * pixelSize) + positionY);


            line.emplace_back(rectangle);
        }
        tiles.emplace_back(line);
    }
}


void Minimap::move(double positionX, double positionY, double width, double height) {
    for (int it = 0; it < TILE_COUNT_WIDTH; it++) {
        for (int jt = 0; jt < TILE_COUNT_HEIGHT; jt++) {
            sf::RectangleShape rectangle = tiles.at(it).at(jt);
            rectangle.setPosition((float(it) * pixelSize) + positionX, (float(jt) * pixelSize) + positionY);
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

void WorldMinimap::draw(int tileX, int tileY, Farm *farm, sf::RenderWindow *window) {
    sf::RectangleShape *rectangle = &tiles.at(tileX).at(tileY);


    float height = farm->getMap()->getTileAt(tileX, tileY);
    RGBColor rectangleColor = RGBColor(0.f, 0.f, ((height + 1) / 2));
    if (height < -0.1f) {
        rectangleColor = RGBColor(0.6f, 1.f, ((height + 1) / 2));
    }
    if (height > 0.7f) {
        rectangleColor = RGBColor(0.59f, 1.f, ((height + 1) / 2));
    }

    rectangle->setFillColor(sf::Color(rectangleColor.getRed(), rectangleColor.getGreen(), rectangleColor.getBlue(), 255));

    window->draw(*rectangle);
}



