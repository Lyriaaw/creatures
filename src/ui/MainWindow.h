//
// Created by Amalric Lombard de Buffières on 7/17/20.
//

#ifndef CREATURES_MAINWINDOW_H
#define CREATURES_MAINWINDOW_H
#include <SFML/Graphics.hpp>
#include <thread>
#include "../farm/Farm.h"
#include "components/Camera.h"
#include "components/BrainUI.h"
#include "components/FarmUI.h"
#include "views/Screen.h"
#include "components/Button.h"


class MainWindow {
public:

    MainWindow();

    void loadFont();
    void loadButtons();
    void loadScreens();

    void loadUI();

    void loadFarm();
    std::thread runFarmLoop();


    void start();
    void runLoop();




private:
    Farm *farm;

    FarmUI *farmUi;
    Camera *mainCamera;

    sf::RenderWindow *window;

    sf::RectangleShape topButtonBackground;

    std::vector<Screen *> screens;
    Screen * currentScreen;



    bool running = false;
    bool paused = false;


    float mouseX, mouseY;
    bool rightMouseButtonDown;
    bool leftMouseButtonDown;

    Life * selectedLife = nullptr;
    Entity * selectedEntity = nullptr;

    BrainUI * brainUi;

    sf::Font * font;

    std::vector<Button *> buttons;

    sf::Text generalInformationLabel;
    sf::Text speedInformationLabel;

    std::chrono::system_clock::time_point tickStart;
    std::chrono::system_clock::time_point tickEnd;
    double fps;


    void handleEvents();
    void handleKeyboardEvents(sf::Event::KeyEvent event);
    void handleScroll(float delta);
    void handleMouseMove(int x, int y);
    void handleResized(int width, int height);
    void handleMousePressed(sf::Mouse::Button button) ;
    void handleMouseReleased(sf::Mouse::Button button) ;
    void draw();

    void handleButtonClicked(int id);

    void openScreen(int id);

    Entity * getSelectedEntity();

    void updateInformationLabel();

    std::thread runChunksLoop();
};


#endif //CREATURES_MAINWINDOW_H
