//
// Created by Amalric Lombard de Buffières on 7/17/20.
//

#ifndef CREATURES_MAINWINDOW_H
#define CREATURES_MAINWINDOW_H
#include <SFML/Graphics.hpp>
#include "../farm/Farm.h"


class MainWindow {
public:

    MainWindow();

    void start();

    bool running = false;

    std::chrono::system_clock::time_point renderDurationStart;
    std::chrono::system_clock::time_point renderDurationEnd;

private:

    Farm farm;

    void runLoop(sf::RenderWindow *window);
    void handleEvents(sf::RenderWindow *window);
    void draw(sf::RenderWindow *window);
};


#endif //CREATURES_MAINWINDOW_H
