//
// Created by Amalric Lombard de Buffières on 10/17/20.
//

#include "TestScreen.h"
#include "../elements/UiButton.h"

TestScreen::TestScreen(FarmUI * farmUi) : Screen(farmUi) {

    UiButton * buttonTest = new UiButton("Test", 0.1, 0.1, 0.2, 0.1);
    buttonTest->setOnClick([]() {
       std::cout << "Clicked !" << std::endl;
    });

    uiComponents.emplace_back(buttonTest);

}

int TestScreen::getId() {
    return 7;
}

void TestScreen::init() {
}


void TestScreen::updateSelectedCreature(Life *connector) {

}




