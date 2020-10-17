//
// Created by Amalric Lombard de Buffières on 10/17/20.
//

#ifndef CREATURES_TESTSCREEN_H
#define CREATURES_TESTSCREEN_H


#include "Screen.h"

class TestScreen: public Screen {
private:
public:
    TestScreen(Farm *farm, sf::Font * font);

    int getId() override;

    void init() override;

    Camera *open() override;

    void draw(sf::RenderWindow *window);

    void updateSelectedCreature(Life *connector) override;


    void loadCamera();
};


#endif //CREATURES_TESTSCREEN_H
