//
// Created by Amalric Lombard de Buffières on 10/17/20.
//

#include "TestScreen.h"

TestScreen::TestScreen(Farm *farm, sf::Font * font) : Screen(farm, font) {}

int TestScreen::getId() {
    return 7;
}

void TestScreen::init() {
//    UiComponent * firstComponent = new UiComponent(0.1, 0.1, 0.5, 0.5);
//    uiComponents.emplace_back(firstComponent);
//
//    UiComponent * secondComponent = new UiComponent(0.9, 0.1, 0.1, 0.8);
//    uiComponents.emplace_back(secondComponent);

    UiComponent * thirdComponent = new UiComponent(0.1, 0.1, 0, 0.8);
    thirdComponent->setFixedSizeAndPosition(0, 0, 100, 0);
    uiComponents.emplace_back(thirdComponent);


}

Camera *TestScreen::open() {
    return nullptr;
}

void TestScreen::draw(sf::RenderWindow *window) {
    for (int it = 0; it < uiComponents.size(); it++) {
        uiComponents.at(it)->draw(window);
    }
}

void TestScreen::updateSelectedCreature(Life *connector) {

}



void TestScreen::onWindowResize(int width, int height) {
    for (int it = 0; it < uiComponents.size(); it++) {
        uiComponents.at(it)->onWindowResized(width, height);
    }
}

void TestScreen::mouseMoved(int x, int y) {
    for (int it = 0; it < uiComponents.size(); it++) {
        uiComponents.at(it)->mouseMoved(x, y);
    }
}

void TestScreen::mouseClicked(int x, int y) {
    for (int it = 0; it < uiComponents.size(); it++) {
        uiComponents.at(it)->mouseClicked(x, y);
    }
}
