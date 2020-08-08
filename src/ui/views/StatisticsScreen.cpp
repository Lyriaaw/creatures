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

    currentGraph = populationGraph;



    Graph * scoreGraph = new Graph("Score");
    scoreGraph->setPosition(0.f, 0.1f, 1.f, 0.8f);

    scoreGraph->addLine(farm->getDataAnalyser().getAverageScore(), 255, 0, 0);
    scoreGraph->addLine(farm->getDataAnalyser().getMedianScore(), 0, 0, 255);
    scoreGraph->addLine(farm->getDataAnalyser().getFirstQuartileScore(), 0, 128, 0);
    scoreGraph->addLine(farm->getDataAnalyser().getLastQuartileScore(), 0, 128, 0);
//    scoreGraph->addLine(farm->getDataAnalyser().getBestScore(), 255, 255, 255);

    scoreGraph->windowResized(windowWidth, windowHeight);


    Graph * timeGraph = new Graph("Time");
    timeGraph->setPosition(0.f, 0.1f, 1.f, 0.8f);

    timeGraph->addLine(farm->getDataAnalyser().getTickTime(), 255, 255, 255);

    timeGraph->addLine(farm->getDataAnalyser().getTotalTime(), 0, 0, 0);

    timeGraph->addLine(farm->getDataAnalyser().getEntityGridTime(), 0, 0, 255);

    timeGraph->addLine(farm->getDataAnalyser().getBrainProcessingTime(), 255, 65, 255);
    timeGraph->addLine(farm->getDataAnalyser().getBrainOutputsTime(), 0, 195, 0);

    timeGraph->addLine(farm->getDataAnalyser().getPrepareActionsTime(), 128, 255, 128);
    timeGraph->addLine(farm->getDataAnalyser().getExecuteActionsTime(), 128, 128, 255);
    timeGraph->addLine(farm->getDataAnalyser().getMoveCreaturesTime(), 255, 128, 128);

    timeGraph->addLine(farm->getDataAnalyser().getPopulationControlTime(), 0, 0, 195);
    timeGraph->addLine(farm->getDataAnalyser().getVegetalisationTime(), 0, 0, 65);

    timeGraph->windowResized(windowWidth, windowHeight);






    graphs.emplace_back(populationGraph);
    graphs.emplace_back(scoreGraph);
    graphs.emplace_back(timeGraph);
}

void StatisticsScreen::loadButtons(sf::Font *font) {
    sf::Color backgroundColor = sf::Color(55, 55, 55, 255);
    sf::Color textColor = sf::Color(195, 195, 195, 255);

    for (int it = 0; it < graphs.size(); it++) {
        Button * button = new Button(graphs.at(it)->getName(), it, font, it * 160, 0, 150, 50, backgroundColor, textColor);
        graphButtons.emplace_back(button);
    }
}


void StatisticsScreen::init(sf::Font *font) {
    loadGraphs();
    loadButtons(font);
}

Camera *StatisticsScreen::open() {
    return nullptr;
}

void StatisticsScreen::draw(sf::RenderWindow *window) {
    currentGraph->draw(window);

    for (int it = 0; it < graphButtons.size(); it++) {
        graphButtons.at(it)->draw(window);
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

    for (int it = 0; it < graphButtons.size(); it++) {
        graphButtons.at(it)->move(graphButtons.at(it)->getX(), 0.9f * height);
    }


}

void StatisticsScreen::mouseMoved(int x, int y) {

}

void StatisticsScreen::mouseClicked(int x, int y) {
    for (int it = 0; it < graphButtons.size(); it++) {
        if (graphButtons.at(it)->clicked(x, y)) {
            clickedOnButton(graphButtons.at(it)->getId());
        }
    }
}

void StatisticsScreen::clickedOnButton(int id) {
    currentGraph = graphs.at(id);
}