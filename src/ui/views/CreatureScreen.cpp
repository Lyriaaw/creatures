//
// Created by Amalric Lombard de Buffières on 10/10/20.
//

#include "CreatureScreen.h"

CreatureScreen::CreatureScreen(Farm *farm, sf::Font * font) : Screen(farm, font) {}

int CreatureScreen::getId() {
    return 4;
}

void CreatureScreen::init() {

}

Camera *CreatureScreen::open() {
    return camera;
}

void CreatureScreen::draw(sf::RenderWindow *window) {

}

void CreatureScreen::updateSelectedCreature(Life *connector) {

}


void CreatureScreen::loadCamera() {
    Point center = Point(FARM_WIDTH / 2.f, FARM_HEIGHT / 2.f);
    Point topLeft = Point(0, 0);

    camera = new Camera(center, topLeft);
    camera->setZoom(.48f);
}

void CreatureScreen::onWindowResize(int width, int height) {
    camera->setWidth(width);
    camera->setHeight(height);
}

void CreatureScreen::mouseMoved(int x, int y) {

}

