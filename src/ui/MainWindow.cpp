//
// Created by Amalric Lombard de Buffières on 7/17/20.
//

#include "MainWindow.h"
#include "../farm/Entity.h"
#include "CreatureUI.h"
#include "FoodUI.h"

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


    for (int it = 0; it < farm->getFoods().size(); it++) {
        Food *entity = farm->getFoods().at(it);

        FoodUI *entityUi = new FoodUI(entity);
        entityUis.push_back(entityUi);
    }

    for (int it = 0; it < farm->getCreatures().size(); it++) {
        Creature *entity = farm->getCreatures().at(it);

        CreatureUI *entityUi = new CreatureUI(entity);
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

void MainWindow::handleScroll(float delta) {


    float deltaRatio;
    if (delta < 0) {
        mainCamera->changeZoom(0.9f);
        deltaRatio = 1.1;
    } else if (delta > 0) {
        mainCamera->changeZoom(1.1f);
        deltaRatio = 0.90909090909;
    } else {
        return;
    }

    Point cameraCenter = mainCamera->getCenter();
    Point mouseWorldCoordinates = mainCamera->getWorldCoordinates({mouseX, mouseY});

    float deltaX = cameraCenter.getX() - mouseWorldCoordinates.getX();
    float deltaY = cameraCenter.getY() - mouseWorldCoordinates.getY();

    float newX = mouseWorldCoordinates.getX() + (deltaX * (deltaRatio));
    float newY = mouseWorldCoordinates.getY() + (deltaY * (deltaRatio));

    Point newCameraCenter = {newX, newY};
    mainCamera->setCenter(newCameraCenter);
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
            case Event::MouseWheelScrolled: {
                handleScroll(event.mouseWheelScroll.delta);
                break;
            }
            case Event::MouseMoved: {
                this->mouseX = float(event.mouseMove.x);
                this->mouseY = float(event.mouseMove.y);
                Point mouseWorldCoordinates = mainCamera->getWorldCoordinates({mouseX, mouseY});
                break;
            }
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
            mainCamera->move(0, -10);
            break;
        case Keyboard::Key::Down:
            mainCamera->move(0, +10);
            break;
        case Keyboard::Key::Left:
            mainCamera->move(-10, 0);
            break;
        case Keyboard::Key::Right:
            mainCamera->move(10, 0);
            break;
        case Keyboard::Key::Space:
            this->paused = !this->paused;
            break;
        case Keyboard::Key::G:
            this->mainCamera->switchGrid();
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

//    cout << "Time: " << elapsed_time.count() << endl;
}


void MainWindow::runLoop() {
    while (running) {
        handleEvents();
        if (!paused) {
            farm->Tick();
        }
        draw();
    }

    window->close();
}
