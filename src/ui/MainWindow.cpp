//
// Created by Amalric Lombard de Buffières on 7/17/20.
//

#include "MainWindow.h"
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

//    mainCamera->setWidth(1920);
//    mainCamera->setHeight(1080);

    mainCamera->setWidth(WINDOW_WIDTH);
    mainCamera->setHeight(WINDOW_HEIGHT);


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
        deltaRatio = 0.9;
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

void MainWindow::handleMouseMove(int x, int y) {
    Point lastMousePosition = mainCamera->getWorldCoordinates({mouseX, mouseY});
    Point newMousePosition = mainCamera->getWorldCoordinates({float(x), float(y)});
    if (rightMouseButtonDown) {

        float deltaX = lastMousePosition.getX() - newMousePosition.getX();
        float deltaY = lastMousePosition.getY() - newMousePosition.getY();

        Point newCenter = {mainCamera->getCenter().getX() + deltaX, mainCamera->getCenter().getY() + deltaY};

        this->mainCamera->setCenter(newCenter);
    }



    this->mouseX = float(x);
    this->mouseY = float(y);

    Entity * globalSelectedEntity = getSelectedEntity();
    if (leftMouseButtonDown && globalSelectedEntity != nullptr) {
        globalSelectedEntity->setPosition({newMousePosition.getX(), newMousePosition.getY()});
    }


}

void MainWindow::handleMousePressed(sf::Mouse::Button button) {
    if (button == Mouse::Right) {
        rightMouseButtonDown = true;
    }
    if (button == Mouse::Left) {
        leftMouseButtonDown = true;
    }

}
void MainWindow::handleMouseReleased(sf::Mouse::Button button) {
    if (button == Mouse::Right) {
        rightMouseButtonDown = false;
    }

    if (button == Mouse::Left) {
        leftMouseButtonDown = false;
    }




    if (button == Mouse::Left) {
        Point worldCoordinates = mainCamera->getWorldCoordinates({mouseX, mouseY});

        bool found = false;
        for (int it = 0; it < farm->getCreatures().size(); it++) {
            Creature * entity = farm->getCreatures().at(it);

            double deltaX = abs(worldCoordinates.getX() - entity->getPosition().getX());
            double deltaY = abs(worldCoordinates.getY() - entity->getPosition().getY());

            if (deltaX < entity->getSize() && deltaY < entity->getSize()) {
                selectedCreature = entity;
                selectedEntity = nullptr;
                found = true;
            }
        }

        for (int it = 0; it < farm->getFoods().size(); it++) {
            Entity * entity = farm->getFoods().at(it);

            double deltaX = abs(worldCoordinates.getX() - entity->getPosition().getX());
            double deltaY = abs(worldCoordinates.getY() - entity->getPosition().getY());

            if (deltaX < entity->getSize() && deltaY < entity->getSize()) {
                selectedEntity = entity;
                selectedCreature = nullptr;
                found = true;
            }
        }

        if (!found) {
            selectedEntity = nullptr;
            selectedCreature = nullptr;
        }



    }
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
                handleMouseMove(event.mouseMove.x, event.mouseMove.y);
                break;
            }
            case Event::MouseButtonPressed:
                handleMousePressed(event.mouseButton.button);
                break;
            case Event::MouseButtonReleased:
                handleMouseReleased(event.mouseButton.button);
                break;
            case Event::Closed:
                running = false;
                break;
            case Event::Resized:
                View view = View(sf::Vector2f(event.size.width / 2.f, event.size.height / 2.f), sf::Vector2f(event.size.width, event.size.height));

                mainCamera->setWidth(event.size.width);
                mainCamera->setHeight(event.size.height);
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
        case Keyboard::Key::C:
            this->selectedEntity = nullptr;
            this->selectedCreature = nullptr;
            break;


    }
}

Entity * MainWindow::getSelectedEntity() {
    if (selectedCreature != nullptr) {
        return selectedCreature;
    }
    if (selectedEntity != nullptr) {
        return selectedEntity;
    }

    return nullptr;
}

void MainWindow::draw() {
    window->clear(sf::Color::Black);

    if (selectedCreature != nullptr && !mainCamera->isShowGrid()) {
        mainCamera->setCenter(selectedCreature->getPosition());
    }

//    Entity * glggobalSelectedEntity = getSelectedEntity();
//    if (globalSelectedEntity != nullptr) {
////        cout << "Selected entity: X:" << globalSelectedEntity->getSimpleCoordinates().getX() << " Y: " << globalSelectedEntity->getSimpleCoordinates().getY() << endl;
//    }

    FarmUI farmUI = farm->getUi();
    farmUI.draw(window, mainCamera, selectedCreature);


    window->display();


    renderDurationEnd = chrono::system_clock::now();
    chrono::duration<double> elapsed_time = renderDurationEnd - renderDurationStart;
    renderDurationStart = chrono::system_clock::now();
}

void MainWindow::runLoop() {
    while (running) {
        handleEvents();
        farm->Tick(paused, selectedCreature);
        draw();
    }

    window->close();
}
