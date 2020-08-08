//
// Created by Amalric Lombard de Buffières on 8/8/20.
//

#include "StatisticsScreen.h"

StatisticsScreen::StatisticsScreen(Farm *farm) : Screen(farm) {
    windowWidth = 0;
    windowHeight = 0;
}

int StatisticsScreen::getId() {
    return 2;
}

void StatisticsScreen::loadGraphs() {
    Graph * populationGraph = new Graph("Population");
    populationGraph->setPosition(0.f, 0.1f, 1.f, 0.8f);

    populationGraph->addLine(farm->getDataAnalyser().getPopulation(), 255, 0, 0);
    populationGraph->windowResized(windowWidth, windowHeight);

    graphs.emplace_back(populationGraph);
}

void StatisticsScreen::init() {
    loadGraphs();
}

Camera *StatisticsScreen::open() {
    return nullptr;
}

void StatisticsScreen::draw(sf::RenderWindow *window) {
    for (int it = 0; it < graphs.size(); it++) {
        graphs.at(it)->draw(window);
    }
}

void StatisticsScreen::updateSelectedCreature(BrainConnector *connector) {

}

void StatisticsScreen::onWindowResize(int width, int height) {
    windowWidth = width;
    windowHeight = height;

    for (int it = 0; it < graphs.size(); it++) {
        graphs.at(it)->windowResized(width, height);
    }
}

void StatisticsScreen::mouseMoved(int x, int y) {

}
