//
// Created by Amalric Lombard de Buffières on 10/17/20.
//

#ifndef CREATURES_TESTSCREEN_H
#define CREATURES_TESTSCREEN_H


#include "Screen.h"

class TestScreen: public Screen {
private:
    std::vector<UiComponent *> uiComponents;
public:
    TestScreen(Farm *farm, sf::Font * font);

    int getId() override;

    void init() override;

    Camera *open() override;

    void draw(sf::RenderWindow *window) override;

    void updateSelectedCreature(Life *connector) override;

    void onWindowResize(int width, int height) override;



    void loadCamera();

    void mouseMoved(int x, int y) override;

    void mouseClicked(int x, int y) override;
};


#endif //CREATURES_TESTSCREEN_H
