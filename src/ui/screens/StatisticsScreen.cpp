//
// Created by Amalric Lombard de Buffières on 8/8/20.
//

#include "StatisticsScreen.h"

StatisticsScreen::StatisticsScreen(FarmUI * farmUi) : Screen(farmUi) {
    windowWidth = 0;
    windowHeight = 0;
}

int StatisticsScreen::getId() {
    return 2;
}

void StatisticsScreen::loadGraphs() {
    Graph * populationGraph = new Graph("Population", 0.f, 0.1f, 1.f, 0.8f);

    populationGraph->addLine(farmUi->getFarm()->getDataAnalyser().getPopulation(), 1, 255, 0, 0);
    populationGraph->addLine(farmUi->getFarm()->getDataAnalyser().getNaturalMatings(), 1, 0, 0, 255);
    populationGraph->windowResized(windowWidth, windowHeight);

    currentGraph = populationGraph;



    Graph * scoreGraph = new Graph("Score", 0.f, 0.1f, 1.f, 0.8f);

    scoreGraph->addLine(farmUi->getFarm()->getDataAnalyser().getAverageScore(), 1, 255, 0, 0);
    scoreGraph->addLine(farmUi->getFarm()->getDataAnalyser().getMedianScore(), 1, 100, 100, 255);
    scoreGraph->addLine(farmUi->getFarm()->getDataAnalyser().getFirstQuartileScore(), 1, 0, 128, 0);
    scoreGraph->addLine(farmUi->getFarm()->getDataAnalyser().getLastQuartileScore(), 1, 0, 128, 0);
    scoreGraph->addLine(farmUi->getFarm()->getDataAnalyser().getBestScore(), 0, 255, 255, 255);

    scoreGraph->windowResized(windowWidth, windowHeight);


    Graph * timeGraph = new Graph("Time", 0.f, 0.1f, 1.f, 0.8f);

    timeGraph->addLine(farmUi->getFarm()->getDataAnalyser().getTickTime(), 2, 255, 255, 255);

    timeGraph->addLine(farmUi->getFarm()->getDataAnalyser().getTotalTime(), 2, 0, 0, 0);

    timeGraph->addLine(farmUi->getFarm()->getDataAnalyser().getEntityGridTime(), 2, 0, 0, 255);
    timeGraph->addLine(farmUi->getFarm()->getDataAnalyser().getCreatureSortingTime(), 2, 255, 0, 255);
    timeGraph->addLine(farmUi->getFarm()->getDataAnalyser().getATickHavePassedTime(), 2, 0, 255, 255);

    timeGraph->addLine(farmUi->getFarm()->getDataAnalyser().getBrainProcessingTime(), 2, 255, 65, 255);

    timeGraph->addLine(farmUi->getFarm()->getDataAnalyser().getMoveCreaturesTime(), 2, 255, 128, 128);
    timeGraph->addLine(farmUi->getFarm()->getDataAnalyser().getExecuteActionsTime(), 2, 128, 128, 255);

    timeGraph->addLine(farmUi->getFarm()->getDataAnalyser().getPopulationControlTime(), 2, 0, 0, 195);
    timeGraph->addLine(farmUi->getFarm()->getDataAnalyser().getVegetalisationTime(), 2, 0, 0, 65);
    timeGraph->addLine(farmUi->getFarm()->getDataAnalyser().getStatisticsTime(), 2, 255, 15, 15);

    timeGraph->windowResized(windowWidth, windowHeight);



    Graph * brainTimeGraph = new Graph("Brain times", 0.f, 0.1f, 1.f, 0.8f);

    brainTimeGraph->addLine(farmUi->getFarm()->getDataAnalyser().getChunkSelection(), 2, 255, 255, 255);

    brainTimeGraph->addLine(farmUi->getFarm()->getDataAnalyser().getSensorProcessing(), 2, 0, 0, 0);

    brainTimeGraph->addLine(farmUi->getFarm()->getDataAnalyser().getBrainProcessing(), 2, 0, 0, 255);

    brainTimeGraph->addLine(farmUi->getFarm()->getDataAnalyser().getExternalActions(), 2, 128, 255, 128);
    brainTimeGraph->addLine(farmUi->getFarm()->getDataAnalyser().getZero(), 2, 0, 0, 0);

    brainTimeGraph->windowResized(windowWidth, windowHeight);







    Graph * energyGraph = new Graph("Energies", 0.f, 0.1f, 1.f, 0.8f);

    energyGraph->addLine(farmUi->getFarm()->getDataAnalyser().getTotalEnergy(), 1, 0, 0, 255);
    energyGraph->addLine(farmUi->getFarm()->getDataAnalyser().getAvailableEnergy(), 1, 100, 100, 255);
    energyGraph->addLine(farmUi->getFarm()->getDataAnalyser().getFoodEnergy(), 1, 0, 255, 0);

    energyGraph->addLine(farmUi->getFarm()->getDataAnalyser().getCreaturesEnergy(), 1, 255, 255, 0);
    energyGraph->addLine(farmUi->getFarm()->getDataAnalyser().getCreaturesMass(), 1, 102, 0, 51);
    energyGraph->addLine(farmUi->getFarm()->getDataAnalyser().getCreaturesWastedEnergy(), 1, 102, 51, 0);

    energyGraph->addLine(farmUi->getFarm()->getDataAnalyser().getHeatEnergy(), 1, 255, 0, 0);
    energyGraph->addLine(farmUi->getFarm()->getDataAnalyser().getGroundEnergy(), 1, 20, 100, 0);

    energyGraph->addLine(farmUi->getFarm()->getDataAnalyser().getZero(), 2, 0, 0, 0);

    energyGraph->windowResized(windowWidth, windowHeight);



    Graph * brainsGraph = new Graph("Brains", 0.f, 0.1f, 1.f, 0.8f);

    brainsGraph->addLine(farmUi->getFarm()->getDataAnalyser().getAverageInputNeurons(), 1, 0, 255, 0);
    brainsGraph->addLine(farmUi->getFarm()->getDataAnalyser().getAverageOutputNeurons(), 1, 0, 0, 255);
    brainsGraph->addLine(farmUi->getFarm()->getDataAnalyser().getAverageLinks(), 1, 255, 255, 255);
    brainsGraph->addLine(farmUi->getFarm()->getDataAnalyser().getZero(), 1, 0, 0, 0);


    brainsGraph->windowResized(windowWidth, windowHeight);





    graphs.emplace_back(populationGraph);
    graphs.emplace_back(scoreGraph);
    graphs.emplace_back(timeGraph);
    graphs.emplace_back(brainTimeGraph);
    graphs.emplace_back(energyGraph);
    graphs.emplace_back(brainsGraph);
}

void StatisticsScreen::loadButtons(sf::Font *font) {
    sf::Color backgroundColor = sf::Color(55, 55, 55, 255);
    sf::Color textColor = sf::Color(195, 195, 195, 255);

    for (int it = 0; it < graphs.size(); it++) {
//        Button * button = new Button(graphs.at(it)->getName(), it, font, it * 160, 0, 150, 50, backgroundColor, textColor);
//        graphButtons.emplace_back(button);
    }
}


void StatisticsScreen::init() {
    loadGraphs();
    loadButtons(font);
}


void StatisticsScreen::draw(sf::RenderWindow *window) {
    currentGraph->draw(window);

    for (int it = 0; it < graphButtons.size(); it++) {
        graphButtons.at(it)->draw(window);
    }
}

void StatisticsScreen::updateSelectedCreature() {

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
    currentGraph->mouseMoved(x, y);
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