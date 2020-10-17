//
// Created by Amalric Lombard de Buffières on 8/8/20.
//

#include "WorldScreen.h"


WorldScreen::WorldScreen(FarmUI *farmUi) : Screen(farmUi) {

}

int WorldScreen::getId() {
    return 1;
}

void WorldScreen::init() {
    loadCamera();
    farmUi->setPositions(camera);
}


void WorldScreen::updateSelectedCreature(Life *connector) {

}


void WorldScreen::loadCamera() {
    Point center = Point(FARM_WIDTH / 2.f, FARM_HEIGHT / 2.f);
    Point topLeft = Point(0, 0);

    camera = new Camera(center, topLeft);
    camera->setZoom(.48f);
}

void WorldScreen::onWindowResize(int width, int height) {
    Screen::onWindowResize(width, height);

    camera->setWidth(width);
    camera->setHeight(height);
}

void WorldScreen::draw(sf::RenderWindow *window) {
    Screen::draw(window);

    farmUi->draw(window, camera, nullptr);


}


