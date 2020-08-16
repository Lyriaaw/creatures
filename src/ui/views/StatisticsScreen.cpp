//
// Created by Amalric Lombard de Buffières on 8/8/20.
//

#include "StatisticsScreen.h"

StatisticsScreen::StatisticsScreen(Farm *farm, sf::Font * font) : Screen(farm, font) {
    windowWidth = 0;
    windowHeight = 0;
}

int StatisticsScreen::getId() {
    return 2;
}

void StatisticsScreen::loadGraphs() {
    Graph * populationGraph = new Graph("Population", font);
    populationGraph->setPosition(0.f, 0.1f, 1.f, 0.8f);

    populationGraph->addLine(farm->getDataAnalyser().getPopulation(), 1, 255, 0, 0);
    populationGraph->addLine(farm->getDataAnalyser().getNaturalMatings(), 1, 0, 0, 255);
    populationGraph->windowResized(windowWidth, windowHeight);

    currentGraph = populationGraph;



    Graph * scoreGraph = new Graph("Score", font);
    scoreGraph->setPosition(0.f, 0.1f, 1.f, 0.8f);

    scoreGraph->addLine(farm->getDataAnalyser().getAverageScore(), 1, 255, 0, 0);
    scoreGraph->addLine(farm->getDataAnalyser().getMedianScore(), 1, 100, 100, 255);
    scoreGraph->addLine(farm->getDataAnalyser().getFirstQuartileScore(), 1, 0, 128, 0);
    scoreGraph->addLine(farm->getDataAnalyser().getLastQuartileScore(), 1, 0, 128, 0);
    scoreGraph->addLine(farm->getDataAnalyser().getBestScore(), 0, 255, 255, 255);

    scoreGraph->windowResized(windowWidth, windowHeight);


    Graph * timeGraph = new Graph("Time", font);
    timeGraph->setPosition(0.f, 0.1f, 1.f, 0.8f);

    timeGraph->addLine(farm->getDataAnalyser().getTickTime(), 2, 255, 255, 255);

    timeGraph->addLine(farm->getDataAnalyser().getTotalTime(), 2, 0, 0, 0);

    timeGraph->addLine(farm->getDataAnalyser().getEntityGridTime(), 2, 0, 0, 255);

    timeGraph->addLine(farm->getDataAnalyser().getPrepareActionsTime(), 2, 128, 255, 128);
    timeGraph->addLine(farm->getDataAnalyser().getBrainProcessingTime(), 2, 255, 65, 255);
    timeGraph->addLine(farm->getDataAnalyser().getBrainOutputsTime(), 2, 0, 195, 0);

    timeGraph->addLine(farm->getDataAnalyser().getMoveCreaturesTime(), 2, 255, 128, 128);
    timeGraph->addLine(farm->getDataAnalyser().getExecuteActionsTime(), 2, 128, 128, 255);

    timeGraph->addLine(farm->getDataAnalyser().getPopulationControlTime(), 2, 0, 0, 195);
    timeGraph->addLine(farm->getDataAnalyser().getVegetalisationTime(), 2, 0, 0, 65);
    timeGraph->addLine(farm->getDataAnalyser().getStatisticsTime(), 2, 255, 15, 15);

    timeGraph->windowResized(windowWidth, windowHeight);



    Graph * brainTimeGraph = new Graph("Brain times", font);
    brainTimeGraph->setPosition(0.f, 0.1f, 1.f, 0.8f);

    brainTimeGraph->addLine(farm->getDataAnalyser().getTickTime(), 2, 255, 255, 255);
    brainTimeGraph->addLine(farm->getDataAnalyser().getChunkSelection(), 2, 0, 255, 0);
    brainTimeGraph->addLine(farm->getDataAnalyser().getTotalGridGeneration(), 2, 255, 0, 0);
    brainTimeGraph->addLine(farm->getDataAnalyser().getSensorProcessing(), 2, 0, 0, 255);
    brainTimeGraph->addLine(farm->getDataAnalyser().getBrainProcessing(), 2, 0, 255, 0);
    brainTimeGraph->addLine(farm->getDataAnalyser().getExternalActions(), 2, 255, 255, 128);

    brainTimeGraph->windowResized(windowWidth, windowHeight);







    Graph * energyGraph = new Graph("Energies", font);
    energyGraph->setPosition(0.f, 0.1f, 1.f, 0.8f);

    energyGraph->addLine(farm->getDataAnalyser().getTotalEnergy(), 1, 0, 0, 255);
    energyGraph->addLine(farm->getDataAnalyser().getAvailableEnergy(), 1, 100, 100, 255);
    energyGraph->addLine(farm->getDataAnalyser().getFoodEnergy(), 1, 0, 255, 0);
    energyGraph->addLine(farm->getDataAnalyser().getCreaturesEnergy(), 1, 255, 0, 0);

    energyGraph->addLine(farm->getDataAnalyser().getHeatEnergy(), 1, 255, 0, 0);
    energyGraph->addLine(farm->getDataAnalyser().getGroundEnergy(), 1, 0, 255, 0);
    energyGraph->addLine(farm->getDataAnalyser().getCreaturesMass(), 1, 255, 255, 0);
    energyGraph->addLine(farm->getDataAnalyser().getEnergyToAdd(), 1, 255, 0, 255);


    energyGraph->windowResized(windowWidth, windowHeight);


    Graph * actionsGraph = new Graph("Actions", font);
    actionsGraph->setPosition(0.f, 0.1f, 1.f, 0.8f);

    actionsGraph->addLine(farm->getDataAnalyser().getTotalActions(), 1, 0, 0, 0);
    actionsGraph->addLine(farm->getDataAnalyser().getCaptureGroundActions(), 1, 0, 255, 0);
    actionsGraph->addLine(farm->getDataAnalyser().getCaptureHeatActions(), 1, 255, 0, 0);
    actionsGraph->addLine(farm->getDataAnalyser().getDuplicateActions(), 1, 0, 0, 255);
    actionsGraph->addLine(farm->getDataAnalyser().getMateActions(), 1, 255, 105, 180);

    actionsGraph->addLine(farm->getDataAnalyser().getEatActions(), 1, 255, 255, 255);


    actionsGraph->windowResized(windowWidth, windowHeight);





    graphs.emplace_back(populationGraph);
    graphs.emplace_back(scoreGraph);
    graphs.emplace_back(timeGraph);
    graphs.emplace_back(brainTimeGraph);
    graphs.emplace_back(energyGraph);
    graphs.emplace_back(actionsGraph);
}

void StatisticsScreen::loadButtons(sf::Font *font) {
    sf::Color backgroundColor = sf::Color(55, 55, 55, 255);
    sf::Color textColor = sf::Color(195, 195, 195, 255);

    for (int it = 0; it < graphs.size(); it++) {
        Button * button = new Button(graphs.at(it)->getName(), it, font, it * 160, 0, 150, 50, backgroundColor, textColor);
        graphButtons.emplace_back(button);
    }
}


void StatisticsScreen::init() {
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

    currentGraph->mouseClicked(x, y);


}

void StatisticsScreen::clickedOnButton(int id) {
    currentGraph = graphs.at(id);
}