//
// Created by Amalric Lombard de Buffières on 7/17/20.
//

#include "MainWindow.h"
#include "CreatureUI.h"
#include "FoodUI.h"
#include "FarmUI.h"
#include "views/WorldScreen.h"
#include "views/StatisticsScreen.h"


#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <random>
#include <iostream>
#include <unistd.h>
#import <thread>
#include <sstream>
#include <iomanip>


int WINDOW_WIDTH = 2560;
int WINDOW_HEIGHT = 1440;

using namespace sf;
using namespace std;



MainWindow::MainWindow(): brainUi(nullptr), leftMouseButtonDown(false), rightMouseButtonDown(false) {
    tickStart = std::chrono::system_clock::now();
    tickEnd = std::chrono::system_clock::now();
}

void MainWindow::loadFont() {
    font = new Font();

    if (!font->loadFromFile("/Users/lyriaaz/projects/perso/Creatures/assets/Montserrat.ttf")) {
        std::cout << "Font not loaded properly !" << std::endl;
    }
}

void MainWindow::loadButtons() {
    sf::Color backgroundColor = sf::Color(50, 50, 50, 255);
    sf::Color textColor = sf::Color(205, 205, 205, 255);

    Button * mainWorldButton = new Button("World", 1, font, 0, 0, 100, 50, backgroundColor, textColor);
    Button * statistics = new Button("Statistics", 2, font, 110, 0, 100, 50, backgroundColor, textColor);

    buttons.emplace_back(mainWorldButton);
    buttons.emplace_back(statistics);
}

void MainWindow::loadFarm() {
    farm = new Farm();
    farm->InitFromRandom();

    farmUi = new FarmUI(farm, font);
    farmUi->loadMap();

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
}

void MainWindow::loadScreens() {
    WorldScreen * worldView = new WorldScreen(farm);
    worldView->init();
    worldView->loadCamera();
    screens.emplace_back(worldView);


    StatisticsScreen * statisticsScreen = new StatisticsScreen(farm);
    statisticsScreen->init();
    screens.emplace_back(statisticsScreen);


    for (int it = 0; it < screens.size(); it++) {
        screens.at(it)->onWindowResize(WINDOW_WIDTH, WINDOW_HEIGHT);
    }

    openScreen(1);
}

void MainWindow::loadUI() {
    topButtonBackground = sf::RectangleShape(sf::Vector2f(FARM_WIDTH, 50));
    topButtonBackground.setPosition(0, 0);

    sf::Color backgroundColor = sf::Color(25, 25, 25, 255);
    topButtonBackground.setFillColor(backgroundColor);
//

    sf::Color textColor = sf::Color(200, 200, 200, 255);
    generalInformationLabel.setFont(*font);
    generalInformationLabel.setString("Loading");
    generalInformationLabel.setCharacterSize(12);
    generalInformationLabel.setFillColor(textColor);
}



void MainWindow::start() {
    window = new RenderWindow(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Creatures");
    window->setVerticalSyncEnabled(true);

    loadFont();
    loadButtons();
    loadFarm();
    loadScreens();
    loadUI();


    farmUi->setPositions(mainCamera);

    running = true;
    std::thread farmThread = runFarmLoop();

    runLoop();
    farmThread.join();
}



std::thread MainWindow::runFarmLoop() {
    auto f = [](Farm *threadedFarm, FarmUI *threadedFarmUI, bool *running, bool *paused){
        while (*running) {
            threadedFarm->Tick(*paused);

            threadedFarmUI->update();
        }
    };

    std::thread farmThread(f, farm, farmUi, &running, &paused);

    return farmThread;
}

void MainWindow::runLoop() {

    while (running) {
        handleEvents();
        updateInformationLabel();
        draw();

        tickEnd = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_time = tickEnd - tickStart;
        tickStart = std::chrono::system_clock::now();

        fps = 1.f / elapsed_time.count();
    }

    window->close();
}

void MainWindow::updateInformationLabel() {


    std::stringstream informationStream;
    informationStream << std::fixed << std::setprecision(2);

    informationStream << "Creatures: " << farm->getDataAnalyser().getPopulation()->getLastValue();
    informationStream << std::endl;

    informationStream << "TPS: " << farm->getDataAnalyser().getTickPerSecond()->getLastValue();
    informationStream << std::endl;

    informationStream << "FPS: " << fps;
    informationStream << std::endl;


    generalInformationLabel.setString(informationStream.str());

    double positionX = window->getSize().x * 0.9;
    double positionY = 5;

    generalInformationLabel.setPosition(positionX, positionY);
}





void MainWindow::draw() {
    window->clear(sf::Color::Black);

    if (mainCamera != nullptr) {
        farmUi->draw(window, mainCamera, selectedCreature);
    }

    if (brainUi != nullptr) {
        brainUi->draw(window);
    }

    currentScreen->draw(window);

    window->draw(topButtonBackground);
    for (int it = 0; it < buttons.size(); it++) {
        buttons.at(it)->draw(window);
    }

    window->draw(generalInformationLabel);

    window->display();
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
                handleResized(event.size.width, event.size.height);
                break;
        }
    }

}

void MainWindow::handleResized(int width, int height) {
    View view = View(sf::Vector2f(width / 2.f, height / 2.f), sf::Vector2f(width, height));

    for (int it = 0; it < screens.size(); it++) {
        screens.at(it)->onWindowResize(width, height);
    }


    window->setView(view);
}

void MainWindow::handleScroll(float delta) {
    if (mainCamera == nullptr) {
        return;
    }

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

    Point worldMousePosition = mainCamera->getWorldCoordinates({float(mouseX), float(mouseY)});

    farmUi->mouseMoved(worldMousePosition, mainCamera);
}

void MainWindow::handleMouseMove(int x, int y) {
    for (int it = 0; it < screens.size(); it++) {
        screens.at(it)->mouseMoved(x, y);
    }

    float previousX = this->mouseX;
    float previousY = this->mouseY;

    this->mouseX = float(x);
    this->mouseY = float(y);


    if (mainCamera == nullptr) {
        return;
    }

    Point lastMousePosition = mainCamera->getWorldCoordinates({previousX, previousY});
    Point newMousePosition = mainCamera->getWorldCoordinates({this->mouseX, this->mouseY});

    if (rightMouseButtonDown) {
        float deltaX = lastMousePosition.getX() - newMousePosition.getX();
        float deltaY = lastMousePosition.getY() - newMousePosition.getY();

        Point newCenter = {mainCamera->getCenter().getX() + deltaX, mainCamera->getCenter().getY() + deltaY};

        this->mainCamera->setCenter(newCenter);
    }




    Entity * globalSelectedEntity = getSelectedEntity();
    if (leftMouseButtonDown && globalSelectedEntity != nullptr) {
        globalSelectedEntity->setPosition({newMousePosition.getX(), newMousePosition.getY()});
    }

    farmUi->mouseMoved(newMousePosition, mainCamera);

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
        for (int it = 0; it < buttons.size(); it++) {
            if (buttons.at(it)->clicked(mouseX, mouseY)) {
                handleButtonClicked(buttons.at(it)->getId());
            }
        }
    }

    if (button == Mouse::Left && mainCamera != nullptr) {
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
                    farmUi->addCreature(entityUi);
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


void MainWindow::handleButtonClicked(int id) {
    switch(id) {
        case 1:
            openScreen(1);
            break;
        case 2:
            openScreen(2);
            break;
        default:
            std::cout << "BUTTON ID NOT FOUND" << std::endl;
            break;

    }
}

void MainWindow::openScreen(int id) {
    Screen * selectedScreen(nullptr);

    for (int it = 0; it < screens.size(); it++) {
        if (screens.at(it)->getId() == id) {
            selectedScreen = screens.at(it);
        }
    }

    if (selectedScreen == nullptr) {
        std::cout << "SCREEN ASKED NOT FOUND WITH ID " << id << std::endl;
        return;
    }

    mainCamera = selectedScreen->open();
    currentScreen = selectedScreen;
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
        case Keyboard::Key::R:
            this->selectedEntity = nullptr;
            this->selectedCreature = nullptr;
            delete brainUi;
            brainUi = nullptr;

            int randomCreatureIndex = rand() % farm->getConnectors().size();
            BrainConnector * creature = farm->getConnectors().at(randomCreatureIndex);

            selectedCreature = creature;

            brainUi = new BrainUI(selectedCreature->getBrain(), window->getSize().x * 0.8, 0, window->getSize().x * 0.2, window->getSize().y, font);



            break;



    }
}

