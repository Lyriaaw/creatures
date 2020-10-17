//
// Created by Amalric Lombard de Buffières on 10/17/20.
//

#include "EvolutionScreen.h"

EvolutionScreen::EvolutionScreen(Farm *farm, sf::Font * font) : Screen(farm, font) {
    windowWidth = 0;
    windowHeight = 0;
}

int EvolutionScreen::getId() {
    return 6;
}

void EvolutionScreen::loadGraphs() {

}

void EvolutionScreen::loadButtons(sf::Font *font) {
    sf::Color backgroundColor = sf::Color(55, 55, 55, 255);
    sf::Color textColor = sf::Color(195, 195, 195, 255);


}


void EvolutionScreen::init() {
    loadGraphs();
    loadButtons(font);
}

Camera *EvolutionScreen::open() {
    return nullptr;
}

void EvolutionScreen::draw(sf::RenderWindow *window) {

}

void EvolutionScreen::updateSelectedCreature(Life *connector) {

}

void EvolutionScreen::onWindowResize(int width, int height) {
    windowWidth = width;
    windowHeight = height;

}

void EvolutionScreen::mouseMoved(int x, int y) {
}

void EvolutionScreen::mouseClicked(int x, int y) {
}

void EvolutionScreen::clickedOnButton(int id) {
}