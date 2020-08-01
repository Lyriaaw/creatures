//
// Created by Amalric Lombard de Buffières on 7/17/20.
//

#ifndef CREATURES_MAINWINDOW_H
#define CREATURES_MAINWINDOW_H
#include <SFML/Graphics.hpp>
#include "../farm/Farm.h"
#include "Camera.h"


class MainWindow {
public:

    MainWindow();

    void start();

    bool running = false;
    bool paused = false;

    std::chrono::system_clock::time_point renderDurationStart;
    std::chrono::system_clock::time_point renderDurationEnd;

    float mouseX, mouseY;
    bool rightMouseButtonDown;

private:

    Farm *farm;
    Camera *mainCamera;

    sf::RenderWindow *window;

    void runLoop();
    void handleEvents();
    void handleKeyboardEvents(sf::Event::KeyEvent event);
    void handleScroll(float delta);
    void handleMouseMove(int x, int y);
    void draw();
};


#endif //CREATURES_MAINWINDOW_H
