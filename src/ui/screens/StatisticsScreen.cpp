//
// Created by Amalric Lombard de Buffières on 8/8/20.
//

#include "StatisticsScreen.h"
#include "../elements/UiButton.h"
#include "../colors/RGBColor.h"

StatisticsScreen::StatisticsScreen(FarmUI * farmUi) : Screen(farmUi) {
    windowWidth = 0;
    windowHeight = 0;
}

int StatisticsScreen::getId() {
    return 2;
}

void StatisticsScreen::loadGraphs() {
    graphControlCenter = new GraphControlCenter();

    Graph * populationGraph = new Graph("Population", 0.f, 0.1f, 1.f, 0.8f);
    populationGraph->setGraphControlCenter(graphControlCenter);

    populationGraph->addLine(farmUi->getFarm()->getDataAnalyser().getPopulation(), 1, 255, 0, 0);
    populationGraph->addLine(farmUi->getFarm()->getDataAnalyser().getNaturalMatings(), 1, 0, 0, 255);
    populationGraph->windowResized(windowWidth, windowHeight);

    currentGraph = populationGraph;



    Graph * scoreGraph = new Graph("Score", 0.f, 0.1f, 1.f, 0.8f);
    scoreGraph->setGraphControlCenter(graphControlCenter);

    scoreGraph->addLine(farmUi->getFarm()->getDataAnalyser().getAverageScore(), 1, 255, 0, 0);
    scoreGraph->addLine(farmUi->getFarm()->getDataAnalyser().getMedianScore(), 1, 0, 0, 255);
    scoreGraph->addLine(farmUi->getFarm()->getDataAnalyser().getFirstQuartileScore(), 1, 0, 255, 0);
    scoreGraph->addLine(farmUi->getFarm()->getDataAnalyser().getLastQuartileScore(), 1, 0, 255, 0);
    scoreGraph->addLine(farmUi->getFarm()->getDataAnalyser().getBestScore(), 0, 255, 255, 255);

    scoreGraph->windowResized(windowWidth, windowHeight);


    Graph * timeGraph = new Graph("Time", 0.f, 0.1f, 1.f, 0.8f);
    timeGraph->setGraphControlCenter(graphControlCenter);

    timeGraph->addLine(farmUi->getFarm()->getDataAnalyser().getTickTime(), 2, 255, 255, 255);
    timeGraph->addLine(farmUi->getFarm()->getMap()->getDataAnalyser().getVegetalisationTime(), 2, 0, 0, 65);

    timeGraph->addLine(farmUi->getFarm()->getDataAnalyser().getTotalTime(), 2, 0, 0, 0);

    timeGraph->addLine(farmUi->getFarm()->getDataAnalyser().getEntityGridTime(), 2, 0, 0, 255);
    timeGraph->addLine(farmUi->getFarm()->getDataAnalyser().getCreatureSortingTime(), 2, 255, 0, 255);
    timeGraph->addLine(farmUi->getFarm()->getDataAnalyser().getATickHavePassedTime(), 2, 0, 255, 255);

    timeGraph->addLine(farmUi->getFarm()->getDataAnalyser().getBrainProcessingTime(), 2, 255, 65, 255);

    timeGraph->addLine(farmUi->getFarm()->getDataAnalyser().getMoveCreaturesTime(), 2, 255, 128, 128);
    timeGraph->addLine(farmUi->getFarm()->getDataAnalyser().getExecuteActionsTime(), 2, 128, 128, 255);

    timeGraph->addLine(farmUi->getFarm()->getDataAnalyser().getPopulationControlTime(), 2, 0, 0, 195);
    timeGraph->addLine(farmUi->getFarm()->getDataAnalyser().getStatisticsTime(), 2, 255, 15, 15);

    timeGraph->windowResized(windowWidth, windowHeight);



    Graph * brainTimeGraph = new Graph("Brain times", 0.f, 0.1f, 1.f, 0.8f);
    brainTimeGraph->setGraphControlCenter(graphControlCenter);

    brainTimeGraph->addLine(farmUi->getFarm()->getDataAnalyser().getChunkSelection(), 2, 255, 255, 255);

    brainTimeGraph->addLine(farmUi->getFarm()->getDataAnalyser().getAccessibleEntities(), 2, 128, 0, 0);
    brainTimeGraph->addLine(farmUi->getFarm()->getDataAnalyser().getSensorProcessing(), 2, 0, 0, 0);

    brainTimeGraph->addLine(farmUi->getFarm()->getDataAnalyser().getBrainProcessing(), 2, 0, 0, 255);

    brainTimeGraph->addLine(farmUi->getFarm()->getDataAnalyser().getExternalActions(), 2, 128, 255, 128);
    brainTimeGraph->addLine(farmUi->getFarm()->getDataAnalyser().getZero(), 2, 0, 0, 0);

    brainTimeGraph->windowResized(windowWidth, windowHeight);







    Graph * energyGraph = new Graph("Energies", 0.f, 0.1f, 1.f, 0.8f);
    energyGraph->setGraphControlCenter(graphControlCenter);

    energyGraph->addLine(farmUi->getFarm()->getBiomassDataTracker().getTotalEnergy(), 1, 0, 0, 255);
    energyGraph->addLine(farmUi->getFarm()->getBiomassDataTracker().getCreaturesAvailableEnergy(), 1, 255, 255, 0);
    energyGraph->addLine(farmUi->getFarm()->getBiomassDataTracker().getCreaturesMass(), 1, 102, 0, 51);
    energyGraph->addLine(farmUi->getFarm()->getBiomassDataTracker().getCreaturesWastedEnergy(), 1, 102, 51, 0);

    energyGraph->addLine(farmUi->getFarm()->getBiomassDataTracker().getEntitiesMass(), 1, 0, 255, 0);

    energyGraph->addLine(farmUi->getFarm()->getBiomassDataTracker().getHeatEnergy(), 1, 255, 0, 0);
    energyGraph->addLine(farmUi->getFarm()->getBiomassDataTracker().getGroundEnergy(), 1, 20, 100, 0);

    energyGraph->windowResized(windowWidth, windowHeight);



    Graph * brainsGraph = new Graph("Brains", 0.f, 0.1f, 1.f, 0.8f);
    brainsGraph->setGraphControlCenter(graphControlCenter);

    brainsGraph->addLine(farmUi->getFarm()->getDataAnalyser().getAverageInputNeurons(), 1, 0, 255, 0);
    brainsGraph->addLine(farmUi->getFarm()->getDataAnalyser().getAverageOutputNeurons(), 1, 0, 0, 255);
    brainsGraph->addLine(farmUi->getFarm()->getDataAnalyser().getAverageLinks(), 1, 255, 255, 255);
    brainsGraph->addLine(farmUi->getFarm()->getDataAnalyser().getZero(), 1, 0, 0, 0);





    Graph * actionsGraph = new Graph("Actions", 0.f, 0.1f, 1.f, 0.8f);
    actionsGraph->setGraphControlCenter(graphControlCenter);


    actionsGraph->addLine(farmUi->getFarm()->getDataAnalyser().getMateSuccessCount(), 2, 0, 0, 255);
    actionsGraph->addLine(farmUi->getFarm()->getDataAnalyser().getMateFailureCount(), 2, 64, 0, 128);

    actionsGraph->addLine(farmUi->getFarm()->getDataAnalyser().getEatLifeCount(), 2, 0, 255, 0);
    actionsGraph->addLine(farmUi->getFarm()->getDataAnalyser().getEatCount(), 2, 0, 128, 0);

    actionsGraph->addLine(farmUi->getFarm()->getDataAnalyser().getBiteLifeCount(), 2, 255, 0, 0);
    actionsGraph->addLine(farmUi->getFarm()->getDataAnalyser().getBiteCount(), 2, 128, 0, 0);


    actionsGraph->addLine(farmUi->getFarm()->getDataAnalyser().getPheromoneCount(), 2, 128, 0, 128);
//    actionsGraph->addLine(farmUi->getFarm()->getDataAnalyser().getPoopCount(), 2, 64, 0, 64);




    graphs.emplace_back(populationGraph);
    graphs.emplace_back(scoreGraph);
    graphs.emplace_back(actionsGraph);
    graphs.emplace_back(timeGraph);
    graphs.emplace_back(brainTimeGraph);
    graphs.emplace_back(energyGraph);
    graphs.emplace_back(brainsGraph);
}

void StatisticsScreen::loadButtons(sf::Font *font) {
    sf::Color backgroundColor = sf::Color(55, 55, 55, 255);
    sf::Color textColor = sf::Color(195, 195, 195, 255);

    double buttonWidth = 0.08;
    double buttonHeight = 0.028;


    for (int it = 0; it < graphs.size(); it++) {
        UiButton * button = new UiButton(it, graphs.at(it)->getName(), it * (buttonWidth + 0.001), 0.04, buttonWidth, buttonHeight);

        button->setOnClick([&](int id) {
            this->currentGraph = graphs.at(id);
        });

        uiComponents.emplace_back(button);
    }
}

void StatisticsScreen::loadGraphControlsButtons() {
    double currentXRatio = 0.7;
    double currentYRatio = 0.04;
    double currentWidthRatio = 0.29;
    double currentHeightRatio = 0.05;

    UiBackground * uiBackground = new UiBackground(currentXRatio, currentYRatio, currentWidthRatio, currentHeightRatio);
    uiBackground->setFillColor(sf::Color(128, 128, 128));
    uiComponents.emplace_back(uiBackground);

    UiButton * allButton = new UiButton(1, "all", currentXRatio + 0.01, currentYRatio + 0.01, 0.025, 0.025);
    allButton->setOnClick([&](int id) {
        graphControlCenter->setAll();
    });
    uiComponents.emplace_back(allButton);

    UiButton * last100TicksButton = new UiButton(1, "-100", currentXRatio + 0.02 + 0.025, currentYRatio + 0.01, 0.025, 0.025);
    last100TicksButton->setOnClick([&](int id) {
        graphControlCenter->setLastTimeFrame(100);
    });
    uiComponents.emplace_back(last100TicksButton);

    UiButton * last1000TicksButton = new UiButton(1, "-1000", currentXRatio + 0.03 + 0.05, currentYRatio + 0.01, 0.025, 0.025);
    last1000TicksButton->setOnClick([&](int id) {
        graphControlCenter->setLastTimeFrame(1000);
    });
    uiComponents.emplace_back(last1000TicksButton);

    UiButton * last10000TicksButton = new UiButton(1, "-10000", currentXRatio + 0.04 + 0.075, currentYRatio + 0.01, 0.025, 0.025);
    last10000TicksButton->setOnClick([&](int id) {
        graphControlCenter->setLastTimeFrame(10000);
    });
    uiComponents.emplace_back(last10000TicksButton);

    UiButton * last100000TicksButton = new UiButton(1, "-100000", currentXRatio + 0.05 + 0.1, currentYRatio + 0.01, 0.025, 0.025);
    last100000TicksButton->setOnClick([&](int id) {
        graphControlCenter->setLastTimeFrame(100000);
    });
    uiComponents.emplace_back(last100000TicksButton);







}


void StatisticsScreen::init() {
    loadGraphs();
    loadButtons(font);
    loadGraphControlsButtons();
}


void StatisticsScreen::draw(sf::RenderWindow *window) {
    currentGraph->draw(window);

    for (int it = 0; it < uiComponents.size(); it++) {
        uiComponents.at(it)->draw(window);
    }
}

void StatisticsScreen::updateSelectedCreature() {

}

void StatisticsScreen::onWindowResize(int width, int height) {
    Screen::onWindowResize(width, height);

    windowWidth = width;
    windowHeight = height;

    for (int it = 0; it < graphs.size(); it++) {
        graphs.at(it)->windowResized(width, height);
    }

    for (int it = 0; it < graphButtons.size(); it++) {
        graphButtons.at(it)->move(graphButtons.at(it)->getX(), 0.9f * height);
    }


}

void StatisticsScreen::mouseMoved(int x, int y, int previousX, int previousY) {
    for (int it = 0; it < uiComponents.size(); it++) {
        uiComponents.at(it)->mouseMoved(x, y, previousX, previousY);
    }

    currentGraph->mouseMoved(x, y, previousX, previousY);
}

void StatisticsScreen::mouseClicked(int x, int y) {
    Screen::mouseClicked(x, y);

    for (int it = 0; it < uiComponents.size(); it++) {
        uiComponents.at(it)->mouseClicked(x, y);
    }

    currentGraph->mouseClicked(x, y);


}

void StatisticsScreen::clickedOnButton(int id) {
    currentGraph = graphs.at(id);
}