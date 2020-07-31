//
// Created by Amalric Lombard de Buffières on 7/17/20.
//

#include "MainWindow.h"
#include "../Entity.h"
#include "EntityUI.h"

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <random>
#include <iostream>



int WINDOW_WIDTH = 2560;
int WINDOW_HEIGHT = 1440;

using namespace sf;
using namespace std;


MainWindow::MainWindow() {



    farm = new Farm();
    farm->InitFromRandom();

    FarmUI *farmUi = new FarmUI();
    farmUi->loadMap(farm->getMap());

    std::vector<EntityUI *> entityUis;
    for (int it = 0; it < farm->getEntities().size(); it++) {
        Entity *entity = farm->getEntities().at(it);

        EntityUI *entityUi = new EntityUI(entity);
        entityUis.push_back(entityUi);


    }
    farmUi->setEntities(entityUis);

    Point center = Point(FARM_WIDTH / 2.f, FARM_HEIGHT / 2.f);
    Point topLeft = Point(0, 0);
    mainCamera = new Camera(center, topLeft);
    mainCamera->setWidth(1920);
    mainCamera->setHeight(1080);
    mainCamera->setZoom(.4f);


    farm->setUi(*farmUi);


}


void MainWindow::start() {

    std::cout << "Fucking shit !" << std::endl;


    std::cout << "Fucking little shit !" << std::endl;

    window = new RenderWindow(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Creatures");
    window->setVerticalSyncEnabled(true);



    running = true;


    runLoop();
}


void MainWindow::handleEvents() {
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

void MainWindow::draw() {
    window->clear(sf::Color::Black);

    FarmUI farmUI = farm->getUi();
    farmUI.draw(window, mainCamera);


    window->display();


    renderDurationEnd = chrono::system_clock::now();
    chrono::duration<double> elapsed_time = renderDurationEnd - renderDurationStart;
    renderDurationStart = chrono::system_clock::now();
}


void MainWindow::runLoop() {
    while (running) {
        handleEvents();
        farm->Tick();
        draw();
    }

    window->close();
}
