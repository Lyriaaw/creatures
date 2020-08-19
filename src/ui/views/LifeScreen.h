//
// Created by Amalric Lombard de Buffières on 8/19/20.
//

#ifndef CREATURES_LIFESCREEN_H
#define CREATURES_LIFESCREEN_H


#include "Screen.h"
#include "../components/BrainUI.h"

class LifeScreen: public Screen {
private:
    BrainUI * brainUi;

    int windowWidth, windowHeight;

    Camera *camera;

    sf::RectangleShape mapBackground;




public:
    LifeScreen(Farm *farm, sf::Font *font);

    int getId() override;

    void init() override;

    Camera *open() override;

    void draw(sf::RenderWindow *window) override;

    void updateSelectedLife(Life * life) override;

    void onWindowResize(int width, int height) override;

    void mouseMoved(int x, int y) override;

    void mouseClicked(int x, int y) override;
};


#endif //CREATURES_LIFESCREEN_H
