//
// Created by Amalric Lombard de Buffières on 8/19/20.
//

#include "LifeScreen.h"

LifeScreen::LifeScreen(Farm *farm, sf::Font *font) : Screen(farm, font), camera(nullptr) {}

int LifeScreen::getId() {
    return 4;
}

void LifeScreen::init() {
    camera = new Camera(Point(0, 0), Point(0, 0));
    camera->setZoom(.48f);


    mapBackground = sf::RectangleShape();
    mapBackground.setFillColor(sf::Color(55, 55, 55, 255));

}

Camera *LifeScreen::open() {
    return camera;
}

void LifeScreen::draw(sf::RenderWindow *window) {
    window->draw(mapBackground);

    if (selectedLife == nullptr || camera == nullptr) {
        return ;
    }

    camera->setCenter(selectedLife->getEntity()->getPosition());



    if (brainUi != nullptr) {
        brainUi->draw(window);
    }

}

void LifeScreen::updateSelectedLife(Life * life) {
    selectedLife = life;
    brainUi = new BrainUI(life->getBrain(), windowWidth * 0.55, 0.1 * windowHeight, 0.40 * windowWidth,  0.8 * windowHeight, font);


}

void LifeScreen::onWindowResize(int width, int height) {
    windowWidth = width;
    windowHeight = height;

    camera->setTopLeft(Point(0.05 * windowWidth, 0.1f * height));
    camera->setWidth(0.45 * width);
    camera->setHeight(0.8f * height);

    mapBackground.setPosition(0.05 * windowWidth, 0.1 * height);
    mapBackground.setSize(sf::Vector2f(0.45 * width, 0.8 * height));



}

void LifeScreen::mouseMoved(int x, int y) {

}

void LifeScreen::mouseClicked(int x, int y) {
    Screen::mouseClicked(x, y);
}
