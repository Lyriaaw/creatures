//
// Created by Amalric Lombard de Buffières on 8/8/20.
//

#ifndef CREATURES_STATISTICSSCREEN_H
#define CREATURES_STATISTICSSCREEN_H


#include "Screen.h"
#include "../dashboards/Graph.h"

class StatisticsScreen: public Screen {
private:
    std::vector<Graph *> graphs;
    int windowWidth, windowHeight;

public:
    StatisticsScreen(Farm *farm);

    int getId() override;

    void init() override;

    Camera *open() override;

    void draw(sf::RenderWindow *window) override;

    void updateSelectedCreature(BrainConnector *connector) override;

    void onWindowResize(int width, int height) override;

    void mouseMoved(int x, int y) override;

    void loadGraphs();

};


#endif //CREATURES_STATISTICSSCREEN_H
