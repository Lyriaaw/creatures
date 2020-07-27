//
// Created by Amalric Lombard de Buffières on 7/17/20.
//

#include "FarmUI.h"
#include "../farm/Farm.h"
#include "../World.h"

FarmUI::FarmUI() {}

void FarmUI::loadMap(Map map) {
    for (int it = 0; it < CHUNK_COUNT_WIDTH; it++) {
        std::vector<sf::RectangleShape> line;
        for (int jt = 0; jt < CHUNK_COUNT_HEIGHT; jt++) {
            float height = map.getTileAt(it, jt);

            sf::RectangleShape rectangle = sf::RectangleShape(sf::Vector2f(CHUNK_SIZE - 2, CHUNK_SIZE - 2));
            rectangle.setPosition((float(it) * CHUNK_SIZE) + 1, (float(jt) * CHUNK_SIZE) + 1);

            if (height > 0) {
                rectangle.setFillColor(sf::Color(100, 100, 100, 255));
            } else {
                rectangle.setFillColor(sf::Color(0, 0, 100, 255));
            }

            line.emplace_back(rectangle);
        }
        tiles.emplace_back(line);
    }
}

void FarmUI::draw(sf::RenderWindow *window) {
    for (int it = 0; it < CHUNK_COUNT_WIDTH; it++) {
        for (int jt = 0; jt < CHUNK_COUNT_HEIGHT; jt++) {
            window->draw(tiles.at(it).at(jt));
        }
    }
}

