//
// Created by Amalric Lombard de Buffières on 8/8/20.
//

#ifndef CREATURES_STATISTICSSCREEN_H
#define CREATURES_STATISTICSSCREEN_H


#include "Screen.h"
#include "../dashboards/Graph.h"
#include "../components/Button.h"
#include "../elements/UiButton.h"

class StatisticsScreen: public Screen {
private:
    std::vector<Graph *> graphs;
    int windowWidth, windowHeight;

    std::vector<Button *> graphButtons;

    Graph * currentGraph;

    GraphControlCenter * graphControlCenter;


public:
    StatisticsScreen(FarmUI * farmUi);

    int getId() override;

    void init() override;

    void draw(sf::RenderWindow *window);

    void updateSelectedCreature() override;

    void onWindowResize(int width, int height);

    void mouseMoved(int x, int y, int previousX, int previousY);

    void loadGraphs();
    void loadButtons(sf::Font *font);

    void mouseClicked(int x, int y);

    void clickedOnButton(int id);

    void loadGraphControlsButtons();
};


#endif //CREATURES_STATISTICSSCREEN_H
