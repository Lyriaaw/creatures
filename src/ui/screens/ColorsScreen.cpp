//
// Created by Amalric Lombard de Buffières on 10/14/20.
//

#include "ColorsScreen.h"
ColorsScreen::ColorsScreen(FarmUI * farmUi) : Screen(farmUi), colorsGraph(nullptr) {}

int ColorsScreen::getId() {
    return 5;
}

void ColorsScreen::init() {
    colorsGraph = new ColorsGraph("Colors", font, farmUi->getFarm()->getDataAnalyser().getColors());
    colorsGraph->setPosition(0.f, 0.1f, 1.f, 0.8f);
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

void ColorsScreen::mouseClicked(int x, int y) {
    colorsGraph->switchMode();
}
