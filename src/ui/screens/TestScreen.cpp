//
// Created by Amalric Lombard de Buffières on 10/17/20.
//

#include "TestScreen.h"
#include "../elements/UiButton.h"

TestScreen::TestScreen(Farm *farm, sf::Font * font) : Screen(farm, font) {

    UiButton * buttonTest = new UiButton("Test", 0.1, 0.1, 0.2, 0.1);
    uiComponents.emplace_back(buttonTest);

}

int TestScreen::getId() {
    return 7;
}

void TestScreen::init() {
}

Camera *TestScreen::open() {
    return nullptr;
}


void TestScreen::updateSelectedCreature(Life *connector) {

}




