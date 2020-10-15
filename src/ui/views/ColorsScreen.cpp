//
// Created by Amalric Lombard de Buffières on 10/14/20.
//

#include "ColorsScreen.h"
ColorsScreen::ColorsScreen(Farm *farm, sf::Font * font) : Screen(farm, font), colorsGraph(nullptr) {}

int ColorsScreen::getId() {
    return 5;
}

void ColorsScreen::init() {
    colorsGraph = new ColorsGraph("Colors", font, farm->getDataAnalyser().getColors());
    colorsGraph->setPosition(0.f, 0.1f, 1.f, 0.8f);
}

Camera *ColorsScreen::open() {
    return nullptr;
}

void ColorsScreen::draw(sf::RenderWindow *window) {
    colorsGraph->draw(window);
}

void ColorsScreen::updateSelectedCreature(Life *connector) {

}


void ColorsScreen::onWindowResize(int width, int height) {
    this->windowWidth = width;
    this->windowHeight = height;
    colorsGraph->windowResized(width, height);
}

void ColorsScreen::mouseMoved(int x, int y) {

}
