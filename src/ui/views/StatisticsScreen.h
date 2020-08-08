//
// Created by Amalric Lombard de Buffières on 8/8/20.
//

#ifndef CREATURES_STATISTICSSCREEN_H
#define CREATURES_STATISTICSSCREEN_H


#include "Screen.h"

class StatisticsScreen: public Screen {
public:
    int getId() override;

    void init() override;

    Camera *open() override;

    void draw(sf::RenderWindow *window) override;

    void updateSelectedCreature(BrainConnector *connector) override;

    void onWindowResize(int width, int height) override;

    void mouseMoved(int x, int y) override;

};


#endif //CREATURES_STATISTICSSCREEN_H
