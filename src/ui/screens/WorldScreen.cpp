//
// Created by Amalric Lombard de Buffières on 8/8/20.
//

#include "WorldScreen.h"
#include "../elements/ViewSettings.h"


WorldScreen::WorldScreen(FarmUI *farmUi) : Screen(farmUi) {
}

int WorldScreen::getId() {
    return 1;
}

void WorldScreen::init() {
    loadCamera();
    farmUi->setPositions(camera);

    ViewSettings * viewSetting = new ViewSettings(0.89, 0.1, 0.1, 0.8);
    viewSetting->setChangeCurrentMinimap([&](Minimap * minimap) {
       farmUi->setCurrentMinimap(minimap);
    });
    uiComponents.emplace_back(viewSetting);
}


void WorldScreen::updateSelectedCreature() {

}


void WorldScreen::loadCamera() {
    Point center = Point(FARM_WIDTH / 2.f, FARM_HEIGHT / 2.f);
    Point topLeft = Point(0, 0);

    camera = new Camera(center, topLeft);
    camera->setZoom(.5f);
}

void WorldScreen::onWindowResize(int width, int height) {
    Screen::onWindowResize(width, height);

    camera->setWidth(width);
    camera->setHeight(height);
    camera->setTopLeft(Point(0, 0.03f * float(height)));
    camera->setWidth(width);
    camera->setHeight(0.97f * float(height));
//    float cameraCenterY = ((0.97f * height) / 2.0f) + (0.03f * height);
//    camera->setCenter(Point(width / 2.0, cameraCenterY));
}

void WorldScreen::draw(sf::RenderWindow *window) {
    farmUi->draw(window, camera, farmUi->getSelectedLife());
    Screen::draw(window);
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

    farmUi->processClick(float(x), float(y), camera);
}

void WorldScreen::mouseScrolled(float delta, int mouseX, int mouseY) {
    Screen::mouseScrolled(delta, mouseX, mouseY);

    float deltaRatio;
    if (delta < 0) {
        camera->changeZoom(0.9f);
        deltaRatio = 1.1;
    } else if (delta > 0) {
        camera->changeZoom(1.1f);
        deltaRatio = 0.9;
    } else {
        return;
    }

    Point cameraCenter = camera->getCenter();
    Point mouseWorldCoordinates = camera->getWorldCoordinates({float(mouseX), float(mouseY)});

    float deltaX = cameraCenter.getX() - mouseWorldCoordinates.getX();
    float deltaY = cameraCenter.getY() - mouseWorldCoordinates.getY();

    float newX = mouseWorldCoordinates.getX() + (deltaX * (deltaRatio));
    float newY = mouseWorldCoordinates.getY() + (deltaY * (deltaRatio));

    Point newCameraCenter = {newX, newY};
    camera->setCenter(newCameraCenter);

    std::cout << camera->getZoom() << std::endl;


    farmUi->mouseMoved(mouseWorldCoordinates, camera);
}

void WorldScreen::handleKeyboardEvents(sf::Event::KeyEvent event) {
    switch (event.code) {
        case sf::Keyboard::Key::Num3:
            this->camera->switchGrid();
            break;
    }
}

