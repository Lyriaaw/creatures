//
// Created by Amalric Lombard de Buffières on 7/17/20.
//

#include "MainWindow.h"
#include "components/FarmUI.h"
#include "screens/WorldScreen.h"
#include "screens/StatisticsScreen.h"
#include "screens/MinimapsScreen.h"
#include "screens/LifeScreen.h"
#include "screens/ColorsScreen.h"
#include "screens/EvolutionScreen.h"
#include "screens/TestScreen.h"
#include "elements/GlobalFont.h"
#include "elements/UiButton.h"
#include "elements/UiBackground.h"


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


MainWindow::MainWindow(): leftMouseButtonDown(false), rightMouseButtonDown(false) {
    tickStart = std::chrono::system_clock::now();
    tickEnd = std::chrono::system_clock::now();
}

void MainWindow::loadFont() {
    sf::Font * font = new Font();

    if (!font->loadFromFile("/Users/lyriaaz/projects/perso/Creatures/assets/Montserrat.ttf")) {
        std::cout << "Font not loaded properly !" << std::endl;
    }

    GlobalFont::MainFont = font;
}

void MainWindow::loadUI() {
    sf::Color backgroundColor = sf::Color(25, 25, 25, 255);

    UiBackground * topBackground = new UiBackground(0, 0, 1, 0.03);
    topBackground->setFillColor(backgroundColor);
    uiComponents.emplace_back(topBackground);

//

    sf::Color textColor = sf::Color(200, 200, 200, 255);
    double positionY = 3;

    double positionX = window->getSize().x * 0.88;
    generalInformationLabel.setFont(*GlobalFont::MainFont);
    generalInformationLabel.setString("Loading");
    generalInformationLabel.setCharacterSize(15);
    generalInformationLabel.setFillColor(textColor);
    generalInformationLabel.setPosition(positionX, positionY);

    positionX = window->getSize().x * 0.94;
    speedInformationLabel.setFont(*GlobalFont::MainFont);
    speedInformationLabel.setString("Loading");
    speedInformationLabel.setCharacterSize(15);
    speedInformationLabel.setFillColor(textColor);
    speedInformationLabel.setPosition(positionX, positionY);
}


void MainWindow::loadButtons() {
    sf::Color backgroundColor = sf::Color(50, 50, 50, 255);
    sf::Color textColor = sf::Color(205, 205, 205, 255);

    double buttonWidth = 0.08;

    double buttonHeight = 0.028;

    UiButton * mainWorldButton = new UiButton("World", 0 * (buttonWidth + 0.001), 0.001, buttonWidth, buttonHeight);
    mainWorldButton->setOnClick([&]() {
        this->openScreen(1);
    });

    UiButton * statistics = new UiButton("Statistics", 1 * (buttonWidth + 0.001),  0.001, buttonWidth, buttonHeight);
    statistics->setOnClick([&] () {
        this->openScreen(2);
    });

    UiButton * minimaps = new UiButton("Minimaps", 2 * (buttonWidth + 0.001),  0.001, buttonWidth, buttonHeight);
    minimaps->setOnClick([&] () {
        this->openScreen(3);
    });

    UiButton * creature = new UiButton("Creatures", 3 * (buttonWidth + 0.001),  0.001, buttonWidth, buttonHeight);
    creature->setOnClick([&] () {
        this->openScreen(4);
    });

    UiButton * colors = new UiButton("Colors", 4 * (buttonWidth + 0.001),  0.001, buttonWidth, buttonHeight);
    colors->setOnClick([&] () {
        this->openScreen(5);
    });

    UiButton * evolutions = new UiButton("Evolutions", 5 * (buttonWidth + 0.001),  0.001, buttonWidth, buttonHeight);
    evolutions->setOnClick([&] () {
        this->openScreen(6);
    });

    UiButton * tests = new UiButton("Tests", 6 * (buttonWidth + 0.001),  0.001, buttonWidth, buttonHeight);
    tests->setOnClick([&] () {
        this->openScreen(7);
    });



    uiComponents.emplace_back(mainWorldButton);
    uiComponents.emplace_back(statistics);
    uiComponents.emplace_back(minimaps);
    uiComponents.emplace_back(creature);
    uiComponents.emplace_back(colors);
    uiComponents.emplace_back(evolutions);
    uiComponents.emplace_back(tests);

}

void MainWindow::loadFarm() {
    farm = new Farm();
    farm->InitFromRandom();

    farmUi = new FarmUI(farm);
}

void MainWindow::loadScreens() {

    WorldScreen * worldView = new WorldScreen(farmUi);
    worldView->init();
    screens.emplace_back(worldView);


//    StatisticsScreen * statisticsScreen = new StatisticsScreen(farmUi);
//    statisticsScreen->init();
//    screens.emplace_back(statisticsScreen);

    MinimapsScreen * minimapsScreen = new MinimapsScreen(farmUi);
    minimapsScreen->init();
    screens.emplace_back(minimapsScreen);

    LifeScreen * creatureScreen = new LifeScreen(farmUi);
    creatureScreen->init();
    screens.emplace_back(creatureScreen);

    ColorsScreen * colorsScreen = new ColorsScreen(farmUi);
    colorsScreen->init();
    screens.emplace_back(colorsScreen);

    EvolutionScreen * evolutionScreen = new EvolutionScreen(farmUi);
    evolutionScreen->init();
    screens.emplace_back(evolutionScreen);

    TestScreen * testScreen = new TestScreen(farmUi);
    testScreen->init();
    screens.emplace_back(testScreen);








    for (int it = 0; it < screens.size(); it++) {
        screens.at(it)->onWindowResize(WINDOW_WIDTH, WINDOW_HEIGHT);
    }

    openScreen(1);
}




void MainWindow::start() {
    window = new RenderWindow(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Creatures");
    window->setVerticalSyncEnabled(true);

    loadFont();
    loadFarm();
    loadUI();
    loadButtons();
    loadScreens();

    for (int it = 0; it < uiComponents.size(); it++) {
        uiComponents.at(it)->onWindowResized(WINDOW_WIDTH, WINDOW_HEIGHT);
    }



    running = true;
    std::thread farmThread = runFarmLoop();
    std::thread farmUiThread = runFarmUiUpdate();

    runLoop();
    farmThread.join();
    farmUiThread.join();
}



std::thread MainWindow::runFarmLoop() {
    auto f = [](Farm *threadedFarm, bool *running, bool *paused){
        while (*running) {
            threadedFarm->Tick(*paused);
        }
    };

    std::thread farmThread(f, farm, &running, &paused);

    return farmThread;
}
std::thread MainWindow::runFarmUiUpdate() {
    auto f = [](FarmUI *threadedFarmUI, bool *running, bool *paused){
        while (*running) {
            threadedFarmUI->update();
        }
    };

    std::thread farmThread(f, farmUi, &running, &paused);

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


    std::stringstream speedStream;
    speedStream << std::fixed << std::setprecision(2);

    speedStream << "TPS: " << farm->getDataAnalyser().getTickPerSecond()->getLastValue();
    speedStream << std::endl;

    speedStream << "FPS: " << fps;
    speedStream << std::endl;

    speedInformationLabel.setString(speedStream.str());



    std::stringstream informationStream;
    informationStream << "Tick: " << farm->getDataAnalyser().getPopulation()->getCount();
    informationStream << std::endl;

    informationStream << "Creatures: " << farm->getDataAnalyser().getPopulation()->getLastValue();
    informationStream << std::endl;

    generalInformationLabel.setString(informationStream.str());
}





void MainWindow::draw() {
    window->clear(sf::Color::Black);


    currentScreen->draw(window);

    window->draw(topButtonBackground);

    for (int it = 0; it < uiComponents.size(); it++) {
        uiComponents.at(it)->draw(window);
    }

    window->draw(generalInformationLabel);
    window->draw(speedInformationLabel);



    window->display();
}




Entity * MainWindow::getSelectedEntity() {
    if (selectedLife != nullptr) {
        return selectedLife->getEntity();
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

    for (int it = 0; it < uiComponents.size(); it++) {
        uiComponents.at(it)->onWindowResized(width, height);
    }




    window->setView(view);
}

void MainWindow::handleScroll(float delta) {
//    if (mainCamera == nullptr) {
//        return;
//    }
//
//    float deltaRatio;
//    if (delta < 0) {
//        mainCamera->changeZoom(0.9f);
//        deltaRatio = 1.1;
//    } else if (delta > 0) {
//        mainCamera->changeZoom(1.1f);
//        deltaRatio = 0.9;
//    } else {
//        return;
//    }
//
//    Point cameraCenter = mainCamera->getCenter();
//    Point mouseWorldCoordinates = mainCamera->getWorldCoordinates({mouseX, mouseY});
//
//    float deltaX = cameraCenter.getX() - mouseWorldCoordinates.getX();
//    float deltaY = cameraCenter.getY() - mouseWorldCoordinates.getY();
//
//    float newX = mouseWorldCoordinates.getX() + (deltaX * (deltaRatio));
//    float newY = mouseWorldCoordinates.getY() + (deltaY * (deltaRatio));
//
//    Point newCameraCenter = {newX, newY};
//    mainCamera->setCenter(newCameraCenter);
//
//    Point worldMousePosition = mainCamera->getWorldCoordinates({float(mouseX), float(mouseY)});
//
//    farmUi->mouseMoved(worldMousePosition, mainCamera);
}

void MainWindow::handleMouseMove(int x, int y) {
    for (int it = 0; it < screens.size(); it++) {
        screens.at(it)->mouseMoved(x, y);
    }

    for (int it = 0; it < uiComponents.size(); it++) {
        uiComponents.at(it)->mouseMoved(x, y);
    }

//    float previousX = this->mouseX;
//    float previousY = this->mouseY;
//
    this->mouseX = float(x);
    this->mouseY = float(y);
//
//
//    if (mainCamera == nullptr) {
//        return;
//    }
//
//    Point lastMousePosition = mainCamera->getWorldCoordinates({previousX, previousY});
//    Point newMousePosition = mainCamera->getWorldCoordinates({this->mouseX, this->mouseY});
//
//    if (rightMouseButtonDown) {
//        float deltaX = lastMousePosition.getX() - newMousePosition.getX();
//        float deltaY = lastMousePosition.getY() - newMousePosition.getY();
//
//        Point newCenter = {mainCamera->getCenter().getX() + deltaX, mainCamera->getCenter().getY() + deltaY};
//
//        this->mainCamera->setCenter(newCenter);
//    }
//
//    if (leftMouseButtonDown) {
//
//        if (selectedEntity != nullptr) {
//            Point newMousePosition = mainCamera->getWorldCoordinates({this->mouseX, this->mouseY});
//
//            selectedEntity->setPosition(newMousePosition);
//        }
//
//
//    }
//
//
//
//    farmUi->mouseMoved(newMousePosition, mainCamera);

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


        for (int it = 0; it < uiComponents.size(); it++) {
            uiComponents.at(it)->mouseClicked(mouseX, mouseY);
        }

        for (int it = 0; it < screens.size(); it++) {
            screens.at(it)->mouseClicked(mouseX, mouseY);
        }

    }

//    if (button == Mouse::Left && mainCamera != nullptr) {
//        Point worldCoordinates = mainCamera->getWorldCoordinates({mouseX, mouseY});
//
//        bool found = false;
//        std::vector<Life *> currentLifes = farm->getLifes();
//        for (int it = 0; it < currentLifes.size(); it++) {
//            Life * connector = currentLifes.at(it);
//
//            double deltaX = abs(worldCoordinates.getX() - connector->getEntity()->getPosition().getX());
//            double deltaY = abs(worldCoordinates.getY() - connector->getEntity()->getPosition().getY());
//
//            if (deltaX < connector->getEntity()->getSize() && deltaY < connector->getEntity()->getSize()) {
//                selectedLife = currentLifes.at(it);
//
//                std::vector<Evolution *>  genome = farm->getNursery()->getEvolutionLibrary().getGenomeFor(selectedLife->getEntity()->getId());
//                std::vector<Neuron *> neurons = selectedLife->getBrain()->getNeurons();
//
//                found = true;
//                updateSelectedCreature();
//            }
//        }
//
//
//        std::vector<Entity *> currentEntities = farm->getEntities();
//        for (int it = 0; it < currentEntities.size(); it++) {
//            Entity * entity = currentEntities.at(it);
//
//            double deltaX = abs(worldCoordinates.getX() - entity->getPosition().getX());
//            double deltaY = abs(worldCoordinates.getY() - entity->getPosition().getY());
//
//            if (deltaX < entity->getSize() && deltaY < entity->getSize()) {
//                selectedEntity = entity;
//
//                found = true;
//                updateSelectedCreature();
//            }
//        }
//
////        for (int it = 0; it < farm->getFoods().size(); it++) {
////            Entity * entity = farm->getFoods().at(it);
////
////            double deltaX = abs(worldCoordinates.getX() - entity->getPosition().getX());
////            double deltaY = abs(worldCoordinates.getY() - entity->getPosition().getY());
////
////            if (deltaX < entity->getSize() && deltaY < entity->getSize()) {
////                selectedEntity = entity;
////                selectedLife = nullptr;
////                found = true;
////            }
////        }
//
//        if (!found) {
//            selectedEntity = nullptr;
//            selectedLife = nullptr;
//        }
//    }


}

void MainWindow::updateSelectedCreature() {
    for (int jt = 0; jt < screens.size(); jt++) {
        screens.at(jt)->updateSelectedCreature(selectedLife);
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
        case 3:
            openScreen(3);
            break;
        case 4:
            openScreen(4);
            break;
        case 5:
            openScreen(5);
            break;
        case 6:
            openScreen(6);
            break;
        case 7:
            openScreen(7);
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

    currentScreen = selectedScreen;
}

void MainWindow::handleKeyboardEvents(Event::KeyEvent event) {
//    switch (event.code) {
//        case Keyboard::Key::Up:
//            mainCamera->move(0, -10);
//            break;
//        case Keyboard::Key::Down:
//            mainCamera->move(0, +10);
//            break;
//        case Keyboard::Key::Left:
//            mainCamera->move(-10, 0);
//            break;
//        case Keyboard::Key::Right:
//            mainCamera->move(10, 0);
//            break;
//        case Keyboard::Key::Space:
//            this->paused = !this->paused;
//            break;
//        case Keyboard::Key::G:
//            this->mainCamera->switchGrid();
//            break;
//        case Keyboard::Key::C:
//            this->selectedEntity = nullptr;
//            this->selectedLife = nullptr;
//            updateSelectedCreature();
//            break;
//        case Keyboard::Key::B:
//            this->selectedEntity = nullptr;
//            this->selectedLife = farm->getScoreSortedCreatures().at(0);
//            if (mainCamera != nullptr) {
//                this->mainCamera->setCenter(selectedLife->getEntity()->getPosition());
//            }
//            updateSelectedCreature();
//            break;
//        case Keyboard::Key::T:
//            if (this->mainCamera == nullptr) {
//                break;
//            }
//            this->mainCamera->setMapMode(0);
//            break;
//        case Keyboard::Key::H:
//            if (this->mainCamera == nullptr) {
//                break;
//            }
//            this->mainCamera->setMapMode(1);
//            break;
//        case Keyboard::Key::S:
//            if (this->mainCamera == nullptr) {
//                break;
//            }
//            this->mainCamera->setMapMode(2);
//            break;
//        case Keyboard::Key::P:
//            if (this->mainCamera == nullptr) {
//                break;
//            }
//            this->mainCamera->setMapMode(3);
//            break;
//        case Keyboard::Key::R:
//            this->selectedEntity = nullptr;
//            this->selectedLife = nullptr;
//
//            int randomCreatureIndex = rand() % farm->getLifes().size();
//            Life * life = farm->getLifes().at(randomCreatureIndex);
//            selectedLife = life;
//            updateSelectedCreature();
//
//            break;
//
//
//
//    }
}

