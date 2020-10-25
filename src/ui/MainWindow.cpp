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

    double positionX = window->getSize().x - (3 * 200);
    generalInformationLabel.setFont(*GlobalFont::MainFont);
    generalInformationLabel.setString("Loading");
    generalInformationLabel.setCharacterSize(15);
    generalInformationLabel.setFillColor(textColor);
    generalInformationLabel.setPosition(positionX, positionY);

    positionX = window->getSize().x - (2 * 200);
    speedInformationLabel.setFont(*GlobalFont::MainFont);
    speedInformationLabel.setString("Loading");
    speedInformationLabel.setCharacterSize(15);
    speedInformationLabel.setFillColor(textColor);
    speedInformationLabel.setPosition(positionX, positionY);

    positionX = window->getSize().x - (1 * 200);
    timeInformationLabel.setFont(*GlobalFont::MainFont);
    timeInformationLabel.setString("Loading");
    timeInformationLabel.setCharacterSize(15);
    timeInformationLabel.setFillColor(textColor);
    timeInformationLabel.setPosition(positionX, positionY);


}


void MainWindow::loadButtons() {
    sf::Color backgroundColor = sf::Color(50, 50, 50, 255);
    sf::Color textColor = sf::Color(205, 205, 205, 255);

    double buttonWidth = 0.08;

    double buttonHeight = 0.028;

    UiButton * mainWorldButton = new UiButton(1, "World", 0 * (buttonWidth + 0.001), 0.001, buttonWidth, buttonHeight);
    mainWorldButton->setOnClick([&](int id) {
        this->openScreen(1);
    });

    UiButton * statistics = new UiButton(1, "Statistics", 1 * (buttonWidth + 0.001),  0.001, buttonWidth, buttonHeight);
    statistics->setOnClick([&] (int id) {
        this->openScreen(2);
    });

    UiButton * minimaps = new UiButton(1, "Minimaps", 2 * (buttonWidth + 0.001),  0.001, buttonWidth, buttonHeight);
    minimaps->setOnClick([&] (int id) {
        this->openScreen(3);
    });

    UiButton * creature = new UiButton(1, "Creatures", 3 * (buttonWidth + 0.001),  0.001, buttonWidth, buttonHeight);
    creature->setOnClick([&] (int id) {
        this->openScreen(4);
    });

    UiButton * colors = new UiButton(1, "Colors", 4 * (buttonWidth + 0.001),  0.001, buttonWidth, buttonHeight);
    colors->setOnClick([&] (int id) {
        this->openScreen(5);
    });

    UiButton * evolutions = new UiButton(1, "Evolutions", 5 * (buttonWidth + 0.001),  0.001, buttonWidth, buttonHeight);
    evolutions->setOnClick([&] (int id) {
        this->openScreen(6);
    });

    UiButton * tests = new UiButton(1, "Tests", 6 * (buttonWidth + 0.001),  0.001, buttonWidth, buttonHeight);
    tests->setOnClick([&] (int id) {
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
    farmUi->setSelectedLifeChanged([&]() {
        for (int it = 0; it < this->screens.size(); it++) {
            screens.at(it)->updateSelectedCreature();
        }
    });


    webUiConnection = new WebUiConnection(farm);
    farm->setTriggerUpdate([&]() {
       webUiConnection->updateFarmClients();
    });

    farm->getMap()->setTriggerUpdate([&]() {
       webUiConnection->updateMapClients();
    });



    webUiConnection->handleTread();
}

void MainWindow::loadScreens() {

    WorldScreen * worldView = new WorldScreen(farmUi);
    worldView->init();
    screens.emplace_back(worldView);


    StatisticsScreen * statisticsScreen = new StatisticsScreen(farmUi);
    statisticsScreen->init();
    screens.emplace_back(statisticsScreen);

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

    farm->getMap()->handleThread();

    runLoop();
    farmThread.join();
    farmUiThread.join();
}



std::thread MainWindow::runFarmLoop() {
    auto f = [](Farm *threadedFarm, bool *running, bool *paused){
        while (threadedFarm->getFarmControl()->isRunning()) {
            if (threadedFarm->getFarmControl()->isPaused()) {
                usleep(1000000);
                continue;
            }


            threadedFarm->Tick(*paused);
        }
    };

    std::thread farmThread(f, farm, &running, &paused);

    return farmThread;
}

std::thread MainWindow::runFarmVegetalisationLoop() {
    auto f = [](Farm *threadedFarm, bool *running, bool *paused){
        while (threadedFarm->getFarmControl()->isRunning()) {
            if (threadedFarm->getFarmControl()->isPaused()) {
                usleep(1000000);
                continue;
            }
            threadedFarm->vegetalisation();


            usleep(10000);


        }
    };

    std::thread farmThread(f, farm, &running, &paused);

    return farmThread;
}




std::thread MainWindow::runFarmUiUpdate() {
    auto f = [](FarmUI *threadedFarmUI, bool *running, bool *paused){
        while (threadedFarmUI->getFarm()->getFarmControl()->isRunning()) {
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

    speedStream <<  "Creatures: " << farm->getDataAnalyser().getPopulation()->getLastValue();
    speedStream <<  std::endl;


    speedStream << "TPS: " << farm->getDataAnalyser().getTickPerSecond()->getAveragedLastValue();
    speedStream << " (" << farm->getDataAnalyser().getTickPerSecond()->getLastValue() << ")";
    speedStream << std::endl;

    speedInformationLabel.setString(speedStream.str());





    int seconds = farm->uptime();




    int hours = seconds / 3600;
    seconds -= hours * 3600;

    int minutes = seconds / 60;
    seconds -= minutes * 60;

    std::stringstream informationStream;
    informationStream << std::fixed << std::setprecision(2);

    informationStream << "Tick: " << farm->getDataAnalyser().getPopulation()->getCount();
    informationStream << std::endl;

    informationStream << "Uptime: ";
    informationStream << hours << "h " << minutes << "m " << seconds << "s ";
    informationStream << std::endl;

    generalInformationLabel.setString(informationStream.str());




    std::stringstream timeInformationStream;
    timeInformationStream << std::fixed << std::setprecision(2);

    timeInformationStream << "FPS: " << fps;
    timeInformationStream << std::endl;

    timeInformationLabel.setString(timeInformationStream.str());


//    timeInformationStream << "Creatures: " << farm->getDataAnalyser().getPopulation()->getLastValue();
//    timeInformationStream << std::endl;



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
    window->draw(timeInformationLabel);



    window->display();
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
    currentScreen->mouseScrolled(delta, mouseX, mouseY);
}

void MainWindow::handleMouseMove(int x, int y) {
    float previousX = this->mouseX;
    float previousY = this->mouseY;

    this->mouseX = float(x);
    this->mouseY = float(y);

    currentScreen->mouseMoved(x, y, previousX, previousY);

    for (int it = 0; it < uiComponents.size(); it++) {
        uiComponents.at(it)->mouseMoved(x, y, previousX, previousY);
    }
}

void MainWindow::handleMousePressed(sf::Mouse::Button button) {
    if (button == Mouse::Right) {
        rightMouseButtonDown = true;
    }
    if (button == Mouse::Left) {
        leftMouseButtonDown = true;
    }

    for (int it = 0; it < screens.size(); it++) {
        screens.at(it)->setMouseButtons(leftMouseButtonDown, rightMouseButtonDown);
    }
}

void MainWindow::handleMouseReleased(sf::Mouse::Button button) {
    if (button == Mouse::Right) {
        rightMouseButtonDown = false;
    }

    if (button == Mouse::Left) {
        leftMouseButtonDown = false;
    }


    for (int it = 0; it < screens.size(); it++) {
        screens.at(it)->setMouseButtons(leftMouseButtonDown, rightMouseButtonDown);
    }


    if (button == Mouse::Left) {
        currentScreen->mouseClicked(mouseX, mouseY);

        for (int it = 0; it < uiComponents.size(); it++) {
            uiComponents.at(it)->mouseClicked(mouseX, mouseY);
        }

        if (currentScreen->getId() == 1) {
            std::cout << "LifesUIs: " << farmUi->getLifeUIs().size() << " ( " << farmUi->getFarm()->getLifes().size() << " ) " << " | EntityUis: " << farmUi->getEntityUIs().size() << " ( " << farmUi->getFarm()->getEntities().size() << " ) " << std::endl;
        }
    }

}

void MainWindow::updateSelectedCreature() {
    for (int jt = 0; jt < screens.size(); jt++) {
        screens.at(jt)->updateSelectedCreature();
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
    currentScreen->handleKeyboardEvents(event);

    switch (event.code) {
        case Keyboard::Key::Space:
            this->farm->getFarmControl()->setPaused(!this->farm->getFarmControl()->isPaused());
            break;
        case Keyboard::Key::Num3:
//            this->mainCamera->switchGrid();
            break;
        case Keyboard::Key::C:
            farmUi->selectedCreatureChange("DISMISS");
            break;
        case Keyboard::Key::B:
            farmUi->selectedCreatureChange("BEST");
            updateSelectedCreature();
            break;
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
        case Keyboard::Key::R:
            farmUi->selectedCreatureChange("RANDOM");
            break;
    }
}

