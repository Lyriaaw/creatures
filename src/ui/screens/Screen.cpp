//
// Created by Amalric Lombard de Buffières on 8/8/20.
//

#include "Screen.h"

Screen::Screen(Farm * farm, sf::Font * font): farm(farm), selectedEntity(nullptr), font(font) {}


void Screen::onWindowResize(int width, int height) {
    for (int it = 0; it < uiComponents.size(); it++) {
        uiComponents.at(it)->onWindowResized(width, height);
    }
}

void Screen::mouseMoved(int x, int y) {
    for (int it = 0; it < uiComponents.size(); it++) {
        uiComponents.at(it)->mouseMoved(x, y);
    }
}

void Screen::mouseClicked(int x, int y) {
    for (int it = 0; it < uiComponents.size(); it++) {
        uiComponents.at(it)->mouseClicked(x, y);
    }
}

void Screen::draw(sf::RenderWindow *window) {
    for (int it = 0; it < uiComponents.size(); it++) {
        uiComponents.at(it)->draw(window);
    }
}

