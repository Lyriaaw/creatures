//
// Created by Amalric Lombard de Buffières on 7/17/20.
//

#include "FarmUI.h"
#include "../farm/Farm.h"

FarmUI::FarmUI(Farm * farm) {
    this->farm = farm;

    for (int it = 0; it < CHUNK_COUNT_WIDTH; it++) {
        for (int jt = 0; jt < CHUNK_COUNT_HEIGHT; jt++) {
            farm->getMapAt(it, jt);
            rectanglesMap[it][jt] = sf::RectangleShape(sf::Vector2f(CHUNK_SIZE - 2, CHUNK_SIZE - 2));
            rectanglesMap[it][jt].setPosition((float(it) * CHUNK_SIZE) + 1, (float(jt) * CHUNK_SIZE) + 1);
        }
    }
}

void FarmUI::draw(sf::RenderWindow *window) {
    for (int it = 0; it < CHUNK_COUNT_WIDTH; it++) {
        for (int jt = 0; jt < CHUNK_COUNT_HEIGHT; jt++) {
            window->draw(rectanglesMap[it][jt]);
        }
    }
}