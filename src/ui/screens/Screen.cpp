//
// Created by Amalric Lombard de Buffières on 8/8/20.
//

#include "Screen.h"

Screen::Screen(FarmUI * farmUi): farmUi(farmUi), rightMouseButtonDown(false), leftMouseButtonDown(false) {}

void Screen::setMouseButtons(bool left, bool right) {
    rightMouseButtonDown = right;
    leftMouseButtonDown = left;
}

void Screen::onWindowResize(int width, int height) {
    for (int it = 0; it < uiComponents.size(); it++) {
        uiComponents.at(it)->onWindowResized(width, height);
    }
}

void Screen::mouseScrolled(float delta, int mouseX, int mouseY) {

}

void Screen::mouseMoved(int x, int y, int previousX, int previousY) {
    for (int it = 0; it < uiComponents.size(); it++) {
        uiComponents.at(it)->mouseMoved(x, y, previousX, previousY);
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

void Screen::updateSelectedCreature() {

}

