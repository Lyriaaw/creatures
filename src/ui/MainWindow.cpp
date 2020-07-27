//
// Created by Amalric Lombard de Buffières on 7/17/20.
//

#include "MainWindow.h"
#include "../Entity.h"

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <random>
#include <iostream>


int WINDOW_WIDTH = 1920;
int WINDOW_HEIGHT = 1080;

using namespace sf;
using namespace std;


MainWindow::MainWindow() {

    farm = Farm();
    farm.InitFromRandom();

    FarmUI farmUi = FarmUI();
    farmUi.loadMap(farm.getMap());


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
            case Event::Closed:
                running = false;
                break;
            case Event::Resized:
                View view = View(sf::Vector2f(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f), sf::Vector2f(event.size.width, event.size.height));
                window->setView(view);
                break;
        }
    }

}

void MainWindow::draw(RenderWindow *window) {
    window->clear(sf::Color::Black);

    std::vector<Entity> currentEntities = farm.getEntities();

    FarmUI farmUI = farm.getUi();
    farmUI.draw(window);

    for (int it = 0; it < currentEntities.size(); it++) {
        currentEntities.at(it).draw(window);
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
