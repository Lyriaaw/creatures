//
// Created by Amalric Lombard de Buffières on 10/14/20.
//

#include "ColorsScreen.h"
ColorsScreen::ColorsScreen(FarmUI * farmUi) : Screen(farmUi), colorsGraph(nullptr) {}

int ColorsScreen::getId() {
    return 5;
}

void ColorsScreen::init() {
    colorsGraph = new ColorsGraph("Colors", 0.f, 0.1f, 1.f, 0.8f, farmUi->getFarm()->getDataAnalyser().getColors());
}

void ColorsScreen::draw(sf::RenderWindow *window) {
    colorsGraph->draw(window);
}

void ColorsScreen::updateSelectedCreature() {

}


void ColorsScreen::onWindowResize(int width, int height) {
    Screen::onWindowResize(width, height);
    colorsGraph->windowResized(width, height);

}


void ColorsScreen::mouseMoved(int x, int y, int previousX, int previousY) {
    Screen::mouseMoved(x, y, previousX, previousY);

}

void ColorsScreen::mouseClicked(int x, int y) {
    Screen::mouseClicked(x, y);

    colorsGraph->switchMode();
}

void ColorsScreen::mouseScrolled(float delta, int mouseX, int mouseY) {
    Screen::mouseScrolled(delta, mouseX, mouseY);


}