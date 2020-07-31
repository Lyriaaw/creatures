//
// Created by Amalric Lombard de Buffières on 7/17/20.
//

#include <iostream>
#include "FarmUI.h"
#include "Camera.h"

FarmUI::FarmUI() {}

void FarmUI::loadMap(Map map) {
    for (int it = 0; it < CHUNK_COUNT_WIDTH; it++) {
        std::vector<sf::RectangleShape> line;
        for (int jt = 0; jt < CHUNK_COUNT_HEIGHT; jt++) {
            float height = map.getTileAt(it, jt);

            sf::RectangleShape rectangle = sf::RectangleShape(sf::Vector2f(CHUNK_SIZE - 2, CHUNK_SIZE - 2));
            rectangle.setPosition((float(it) * CHUNK_SIZE) + 1, (float(jt) * CHUNK_SIZE) + 1);

            if (height > 0) {
                rectangle.setFillColor(sf::Color(128, 128, 128, 255));
            } else {
                rectangle.setFillColor(sf::Color(0, 9, 128, 255));
            }

            line.emplace_back(rectangle);
        }
        tiles.emplace_back(line);
    }
}

void FarmUI::setPositions(Camera *camera) {
//     std::cout << "Setting positions " << camera->getZoom() << std::endl;

    for (int it = 0; it < CHUNK_COUNT_WIDTH; it++) {
        std::vector<sf::RectangleShape> line;
        for (int jt = 0; jt < CHUNK_COUNT_HEIGHT; jt++) {

            sf::RectangleShape *rectangle = &tiles.at(it).at(jt);

            rectangle->setSize(sf::Vector2f(CHUNK_SIZE * camera->getZoom(), CHUNK_SIZE * camera->getZoom()));

            Point point = camera->getScreenCoordinates({float(it) * CHUNK_SIZE, float(jt) * CHUNK_SIZE});

            rectangle->setPosition(point.getX(), point.getY());


        }
    }
}

void FarmUI::draw(sf::RenderWindow *window, Camera *camera) {
    setPositions(camera);

    for (int it = 0; it < CHUNK_COUNT_WIDTH; it++) {
        for (int jt = 0; jt < CHUNK_COUNT_HEIGHT; jt++) {
            window->draw(tiles.at(it).at(jt));
        }
    }

    for (int it = 0; it < entities.size(); it++) {
        entities.at(it)->draw(window, camera);
    }

}

void FarmUI::setEntities(const std::vector<EntityUI *> entities) {
    FarmUI::entities = entities;
}


