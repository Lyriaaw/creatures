//
// Created by Amalric Lombard de Buffières on 8/8/20.
//

#include "WorldScreen.h"

WorldScreen::WorldScreen(Farm *farm) : Screen(farm) {}

int WorldScreen::getId() {
    return 1;
}

void WorldScreen::init(sf::Font *font) {

}

Camera *WorldScreen::open() {
    return camera;
}

void WorldScreen::draw(sf::RenderWindow *window) {

}

void WorldScreen::updateSelectedCreature(BrainConnector *connector) {

}


void WorldScreen::loadCamera() {
    Point center = Point(FARM_WIDTH / 2.f, FARM_HEIGHT / 2.f);
    Point topLeft = Point(0, 0);

    camera = new Camera(center, topLeft);
    camera->setZoom(.48f);
}

void WorldScreen::onWindowResize(int width, int height) {
    camera->setWidth(width);
    camera->setHeight(height);
}

void WorldScreen::mouseMoved(int x, int y) {

}
