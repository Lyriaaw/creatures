//
// Created by Amalric Lombard de Buffières on 8/8/20.
//

#ifndef CREATURES_STATISTICSSCREEN_H
#define CREATURES_STATISTICSSCREEN_H


#include "Screen.h"
#include "../dashboards/Graph.h"
#include "../components/Button.h"

class StatisticsScreen: public Screen {
private:
    std::vector<Graph *> graphs;
    int windowWidth, windowHeight;

    std::vector<Button *> graphButtons;

    Graph * currentGraph;

public:
    StatisticsScreen(Farm *farm, sf::Font * font);

    int getId() override;

    void init() override;

    Camera *open() override;

    void draw(sf::RenderWindow *window) override;

    void updateSelectedCreature(Life *connector) override;

    void onWindowResize(int width, int height) override;

    void mouseMoved(int x, int y) override;

    void loadGraphs();
    void loadButtons(sf::Font *font);

    void mouseClicked(int x, int y) override;

    void clickedOnButton(int id);

};


#endif //CREATURES_STATISTICSSCREEN_H
