//
// Created by Amalric Lombard de Buffières on 8/8/20.
//

#include "WorldScreen.h"

int WorldScreen::getId() {
    return 0;
}

void WorldScreen::init() {

}

Camera *WorldScreen::open() {
    return camera;
}

void WorldScreen::draw() {

}

void WorldScreen::updateSelectedCreature(BrainConnector *connector) {

}


void WorldScreen::loadCamera() {
    Point center = Point(FARM_WIDTH / 2.f, FARM_HEIGHT / 2.f);
    Point topLeft = Point(0, 0);

    camera = new Camera(center, topLeft);
    camera->setZoom(.4f);
}

void WorldScreen::onWindowResize(int width, int height) {
    camera->setWidth(width);
    camera->setHeight(height);
}

void WorldScreen::mouseMoved(int x, int y) {

}
