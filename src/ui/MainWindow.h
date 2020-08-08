//
// Created by Amalric Lombard de Buffières on 7/17/20.
//

#ifndef CREATURES_MAINWINDOW_H
#define CREATURES_MAINWINDOW_H
#include <SFML/Graphics.hpp>
#include <thread>
#include "../farm/Farm.h"
#include "Camera.h"
#include "BrainUI.h"
#include "FarmUI.h"
#include "views/Screen.h"


class MainWindow {
public:

    MainWindow();

    void loadFont();
    void loadViews();

    void loadFarm();
    std::thread runFarmLoop();


    void start();
    void runLoop();




private:
    Farm *farm;

    FarmUI *farmUi;
    Camera *mainCamera;

    sf::RenderWindow *window;


    std::vector<Screen *> screens;


    bool running = false;
    bool paused = false;


    float mouseX, mouseY;
    bool rightMouseButtonDown;
    bool leftMouseButtonDown;

    BrainConnector * selectedCreature = nullptr;
    Entity * selectedEntity = nullptr;

    BrainUI * brainUi;

    sf::Font * font;



    void handleEvents();
    void handleKeyboardEvents(sf::Event::KeyEvent event);
    void handleScroll(float delta);
    void handleMouseMove(int x, int y);
    void handleResized(int width, int height);
    void handleMousePressed(sf::Mouse::Button button) ;
    void handleMouseReleased(sf::Mouse::Button button) ;
    void draw();

    Entity * getSelectedEntity();
};


#endif //CREATURES_MAINWINDOW_H
