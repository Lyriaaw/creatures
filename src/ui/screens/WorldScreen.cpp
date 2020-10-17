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

void WorldScreen::mouseMoved(int x, int y, int previousX, int previousY) {
    Screen::mouseMoved(x, y, previousX, previousY);

    Point lastMousePosition = camera->getWorldCoordinates({float(previousX), float(previousY)});
    Point newMousePosition = camera->getWorldCoordinates({float(x), float(y)});


    if (rightMouseButtonDown) {
        float deltaX = lastMousePosition.getX() - newMousePosition.getX();
        float deltaY = lastMousePosition.getY() - newMousePosition.getY();

        Point newCenter = {camera->getCenter().getX() + deltaX, camera->getCenter().getY() + deltaY};

        this->camera->setCenter(newCenter);
    }

//    if (leftMouseButtonDown) {
//
//        if (selectedEntity != nullptr) {
//            Point newMousePosition = camera->getWorldCoordinates({this->mouseX, this->mouseY});
//            selectedEntity->setPosition(newMousePosition);
//        }
//    }

    farmUi->mouseMoved(newMousePosition, camera);
}

void WorldScreen::mouseClicked(int x, int y) {
    Screen::mouseClicked(x, y);
}


