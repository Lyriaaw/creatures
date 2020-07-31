//
// Created by Amalric Lombard de Buffières on 7/17/20.
//

#include "MainWindow.h"
#include "../Entity.h"
#include "../UiEntity.h"

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <random>
#include <iostream>



int WINDOW_WIDTH = 2560;
int WINDOW_HEIGHT = 1440;

using namespace sf;
using namespace std;


MainWindow::MainWindow() {

    farm = Farm();
    farm.InitFromRandom();

    for (int it = 0; it < farm.getEntities().size(); it++) {
        Entity currentEntity = farm.getEntities().at(it);

        auto currentEntityUI =  UiEntity();
        currentEntityUI.initialize(currentEntity.getColor());

        currentEntity.setEntityUi(currentEntityUI);

    }

    FarmUI farmUi = FarmUI();
    farmUi.loadMap(farm.getMap());

    Point center = Point(FARM_WIDTH / 2.f, FARM_HEIGHT / 2.f);
    Point topLeft = Point(0, 0);
    mainCamera = new Camera(center, topLeft);
    mainCamera->setWidth(1920);
    mainCamera->setHeight(1080);
    mainCamera->setZoom(.4f);


    farm.setUi(farmUi);

}


void MainWindow::start() {

    RenderWindow window(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Creatures");
    window.setVerticalSyncEnabled(true);

    running = true;

    renderDurationStart = chrono::system_clock::now();
    renderDurationEnd = chrono::system_clock::now();


    runLoop(&window);
}


void MainWindow::handleEvents(RenderWindow *window) {
    // check all the window's events that were triggered since the last iteration of the loop
    Event event;
    while (window->pollEvent(event))
    {
        switch (event.type) {
            case Event::KeyPressed:
                handleKeyboardEvents(event.key);
                break;
            case Event::MouseWheelScrolled:
                if (event.mouseWheelScroll.delta < 0) {
                    mainCamera->changeZoom(0.9f);
                } else if (event.mouseWheelScroll.delta > 0) {
                    mainCamera->changeZoom(1.1f);
                }

                break;
            case Event::Closed:
                running = false;
                break;
            case Event::Resized:
                View view = View(sf::Vector2f(event.size.width / 2.f, event.size.height / 2.f), sf::Vector2f(event.size.width, event.size.height));
                window->setView(view);
                break;
        }
    }

}

void MainWindow::handleKeyboardEvents(Event::KeyEvent event) {
    switch (event.code) {
        case Keyboard::Key::Up:
            mainCamera->move(0, 10);
            break;
        case Keyboard::Key::Down:
            mainCamera->move(0, -10);
            break;
        case Keyboard::Key::Left:
            mainCamera->move(-10, 0);
            break;
        case Keyboard::Key::Right:
            mainCamera->move(10, 0);
            break;
    }
}

void MainWindow::draw(RenderWindow *window) {
    window->clear(sf::Color::Black);

    std::vector<Entity> currentEntities = farm.getEntities();

    FarmUI farmUI = farm.getUi();
    farmUI.draw(window, mainCamera);

    for (auto & currentEntity : currentEntities) {
        UiEntity currentEntityUI = currentEntity.getEntityUi();

        currentEntityUI.draw(window, mainCamera, currentEntity.getPosition(), currentEntity.getSize(), currentEntity.getRotation(), currentEntity.getColor());
    }

    window->display();


    renderDurationEnd = chrono::system_clock::now();
    chrono::duration<double> elapsed_time = renderDurationEnd - renderDurationStart;
    renderDurationStart = chrono::system_clock::now();
}


void MainWindow::runLoop(RenderWindow *window) {
    while (running) {
        handleEvents(window);
        farm.Tick();
        draw(window);
    }

    window->close();
}
