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


    for (int it = 0; it < farm->getConnectors().size(); it++) {
        Creature *entity = farm->getConnectors().at(it)->getCreature();

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


    farm->setUi(farmUi);


    ticksCount = 0;
    tickTimeTotal = 0;


}


void MainWindow::start() {

    font = new Font();
//
    if (!font->loadFromFile("/Users/lyriaaz/projects/perso/Creatures/assets/Montserrat.ttf")) {
        std::cout << "Font not loaded properly !" << std::endl;
        return;
    }


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
        for (int it = 0; it < farm->getConnectors().size(); it++) {
            BrainConnector * connector = farm->getConnectors().at(it);

            double deltaX = abs(worldCoordinates.getX() - connector->getCreature()->getPosition().getX());
            double deltaY = abs(worldCoordinates.getY() - connector->getCreature()->getPosition().getY());

            if (deltaX < connector->getCreature()->getSize() && deltaY < connector->getCreature()->getSize()) {
                if (selectedCreature != nullptr) {
                    BrainConnector * newCreature = farm->getNursery()->Mate(selectedCreature, connector);

                    farm->addConnector(newCreature);

                    CreatureUI *entityUi = new CreatureUI(newCreature->getCreature());
                    farm->getUi()->addCreature(entityUi);
                }

                selectedCreature = farm->getConnectors().at(it);

                std::vector<Evolution *>  genome = farm->getNursery()->getEvolutionLibrary().getGenomeFor(selectedCreature->getCreature()->getId());
                std::vector<Neuron *> neurons = selectedCreature->getBrain()->getNeurons();

                if (brainUi != nullptr) {
                    delete brainUi;
                }
                brainUi = new BrainUI(selectedCreature->getBrain(), window->getSize().x * 0.8, 0, window->getSize().x * 0.2, window->getSize().y, font);


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
            delete brainUi;
            brainUi = nullptr;
            break;


    }
}

Entity * MainWindow::getSelectedEntity() {
    if (selectedCreature != nullptr) {
        return selectedCreature->getCreature();
    }
    if (selectedEntity != nullptr) {
        return selectedEntity;
    }

    return nullptr;
}

void MainWindow::draw() {
    window->clear(sf::Color::Black);

    if (selectedCreature != nullptr && !mainCamera->isShowGrid()) {
        mainCamera->setCenter(selectedCreature->getCreature()->getPosition());
    }

//    Entity * glggobalSelectedEntity = getSelectedEntity();
//    if (globalSelectedEntity != nullptr) {
////        cout << "Selected entity: X:" << globalSelectedEntity->getSimpleCoordinates().getX() << " Y: " << globalSelectedEntity->getSimpleCoordinates().getY() << endl;
//    }

    FarmUI * farmUI = farm->getUi();
    farmUI->draw(window, mainCamera, selectedCreature);

    if (brainUi != nullptr) {
        brainUi->draw(window);
    }


    window->display();


}

void MainWindow::runLoop() {
    while (running) {
        handleEvents();
        farm->Tick(paused);
        draw();



        renderDurationEnd = chrono::system_clock::now();
        chrono::duration<double> elapsed_time = renderDurationEnd - renderDurationStart;

        ticksCount++;
        tickTimeTotal += elapsed_time.count();

        if (ticksCount % 100 == 0) {
            std::cout << "Average on last 100 ticks: " << 1.f /(tickTimeTotal / 100.f) << " tps. Average available entities: " << farm->getAverageSelectedEntities() << "," << std::endl;
            tickTimeTotal = 0;
        }

        renderDurationStart = chrono::system_clock::now();
    }

    window->close();
}
