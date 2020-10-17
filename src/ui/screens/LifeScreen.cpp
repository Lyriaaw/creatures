//
// Created by Amalric Lombard de Buffières on 10/10/20.
//


#include <sstream>
#include "LifeScreen.h"
#include "../elements/GlobalFont.h"
#include "../elements/UiButton.h"

LifeScreen::LifeScreen(FarmUI * farmUi) : Screen(farmUi), camera(nullptr), informationToShow("BRAIN"), brainUi(nullptr), creatureAgeText(nullptr) {
    creatureAgeText = new UiLabel(0.01, 0.92, 0, 0);
    uiComponents.emplace_back(creatureAgeText);

}

int LifeScreen::getId() {
    return 4;
}

void LifeScreen::init() {
    camera = new Camera(Point(0, 0), Point(0, 0));
    camera->setZoom(.48f);


    mapBackground = sf::RectangleShape();
    mapBackground.setFillColor(sf::Color(55, 55, 55, 255));


    sf::Color backgroundColor = sf::Color(55, 55, 55, 255);
    sf::Color textColor = sf::Color(200, 200, 200, 255);

    double buttonWidth = 0.08;
    double buttonHeight = 0.028;

    UiButton * genomeButton = new UiButton(1, "Brain", 0 * (buttonWidth + 0.001), 0.04, buttonWidth, buttonHeight);
    genomeButton->setOnClick([&](int id){ informationToShow = "BRAIN";});
    UiButton * brainButton = new UiButton(2, "Genome", 1 * (buttonWidth + 0.001), 0.04, buttonWidth, buttonHeight);
    brainButton->setOnClick([&](int id){ informationToShow = "GENOME";});
    UiButton * graphButton = new UiButton(3, "Graphs", 2 * (buttonWidth + 0.001), 0.04, buttonWidth, buttonHeight);
    graphButton->setOnClick([&](int id){ informationToShow = "GRAPHS";});

    uiComponents.emplace_back(brainButton);
    uiComponents.emplace_back(genomeButton);
    uiComponents.emplace_back(graphButton);

    creatureAgeText->setText("Creature age ...");
    creatureAgeText->setFillColor(textColor);

}


void LifeScreen::draw(sf::RenderWindow *window) {
    Screen::draw(window);




    window->draw(mapBackground);


    for (int it = 0; it < buttons.size(); it++) {
        buttons.at(it).draw(window);
    }

    farmUi->draw(window, camera, farmUi->getSelectedLife());

    if (selectedEntity == nullptr || camera == nullptr) {
        return ;
    }
    updateGraphs();


    std::stringstream ageStream;

    ageStream << "Age: " << selectedEntity->getEntity()->getAge();
    creatureAgeText->setText(ageStream.str());


    camera->setCenter(selectedEntity->getEntity()->getPosition());



    if (informationToShow == "BRAIN") {
        if (brainUi != nullptr) {
            brainUi->draw(window);
        }
    }

    if (informationToShow == "GENOME") {
        drawGenome(window);
    }

    if (informationToShow == "GRAPHS") {
        drawGraphs(window);
    }



}

void LifeScreen::drawGenome(sf::RenderWindow *window) {
    for (int it = 0; it < genomeTexts.size(); it++) {
        window->draw(genomeTexts.at(it));
    }
}


void LifeScreen::updateSelectedCreature() {
    selectedEntity = farmUi->getSelectedLife();

    if (selectedEntity == nullptr) {
        brainUi = nullptr;
    } else {
        brainUi = new BrainUI(selectedEntity->getBrain(), windowWidth * 0.55, 0.1 * windowHeight, 0.44 * windowWidth,  0.8 * windowHeight, font, windowWidth, windowHeight);
        loadSelectedGenome();
        loadSelectedGraphs();
    }


}

void LifeScreen::loadSelectedGenome() {
    genomeTexts.clear();


    std::vector<Evolution *> genome = farmUi->getFarm()->getNursery()->getEvolutionLibrary().getGenomeFor(selectedEntity->getEntity()->getId());
    sf::Color textColor = sf::Color(200, 200, 200, 255);

    int displayedCount = 0;
    for (int it = 0; it < genome.size(); it++) {
        if (genome.at(it)->describe().find("Link:") != std::string::npos) {
            continue;
        }
        displayedCount++;


        sf::Text currentText;
        currentText.setString(genome.at(it)->describe());
        currentText.setFont(*GlobalFont::MainFont);
        currentText.setFillColor(textColor);
        currentText.setCharacterSize(20);

        int x = 0.6 * windowWidth;
        int y = (0.1 * windowHeight) + (displayedCount * 30);

        currentText.setPosition(x, y);

        genomeTexts.emplace_back(currentText);
    }

}

void LifeScreen::onWindowResize(int width, int height) {
    Screen::onWindowResize(width, height);

    windowWidth = width;
    windowHeight = height;

    camera->setTopLeft(Point(0.01 * windowWidth, 0.1f * height));
    camera->setWidth(0.49 * width);
    camera->setHeight(0.8f * height);

    mapBackground.setPosition(0.01 * windowWidth, 0.1 * height);
    mapBackground.setSize(sf::Vector2f(0.49 * width, 0.8 * height));


    for (int it = 0; it < buttons.size(); it++) {
        buttons.at(it).move(buttons.at(it).getX(), height - 100);
    }
}


void LifeScreen::mouseClicked(int x, int y) {
    Screen::mouseClicked(x, y);


    for (int it = 0; it < buttons.size(); it++) {
        if (buttons.at(it).clicked(x, y)) {
            handleButtonClick(buttons.at(it).getId());
        }
    }


    if (brainUi != nullptr) {
        if (brainUi->mouseClicked(x, y)) {
            return;
        }
    }

    farmUi->processClick(float(x), float(y), camera);

}

void LifeScreen::handleButtonClick(int id) {
    switch (id) {
        case 1:
            informationToShow = "BRAIN";
            break;
        case 2:
            informationToShow = "GENOME";
            break;
        case 3:
            informationToShow = "GRAPHS";
            break;

            break;
    }
}

void LifeScreen::loadSelectedGraphs() {
    grad0 = new DataItem("0", true);
    grad1 = new DataItem("1", true);
    gradm1 = new DataItem("-1", true);

    inputNeuronsGraph = new Graph("InputNeuron", 0.6f, 0.1f, 0.35f, 0.2f);





    bioGraph = new Graph("Bio", 0.55f, 0.41f, 0.4f, 0.2f);
    bioGraph->windowResized(windowWidth, windowHeight);

    energy = new DataItem("energy", true);
    mass = new DataItem("mass", true);
    maxEnergy = new DataItem("maxEnergy", true);
    wasted = new DataItem("wasted", true);

    bioGraph->addLine(energy, 1, 255, 255, 0);
    bioGraph->addLine(maxEnergy, 1, 255, 255, 255);
    bioGraph->addLine(mass, 1, 102, 0, 51);
    bioGraph->addLine(wasted, 1, 102, 51, 0);
    bioGraph->addLine(grad0, 1, 0, 0, 0);
    bioGraph->windowResized(windowWidth, windowHeight);




    outputNeuronsGraph = new Graph("Output Neurons", 0.55f, 0.1f, 0.4f, 0.2f);
    outputNeuronsGraph->windowResized(windowWidth, windowHeight);


    movement = new DataItem("movement", true);
    rotation = new DataItem("rotation", true);
    mouth = new DataItem("mouth", true);
    genitals = new DataItem("genitals", true);

    outputNeuronsGraph->addLine(movement, 1, 255, 255, 255);
    outputNeuronsGraph->addLine(rotation, 1, 0, 0, 0);
    outputNeuronsGraph->addLine(mouth, 1, 0, 255, 0);
    outputNeuronsGraph->addLine(genitals, 1, 0, 0, 255);


    outputNeuronsGraph->addLine(grad0, 1, 50, 50, 50);
    outputNeuronsGraph->addLine(grad1, 1, 50, 50, 50);
    outputNeuronsGraph->addLine(gradm1, 1, 50, 50, 50);

    outputNeuronsGraph->windowResized(windowWidth, windowHeight);


}

void LifeScreen::updateGraphs() {
    if (selectedEntity == nullptr) {
        return;
    }

    movement->addValue(selectedEntity->getBrain()->getOutputNeurons().at(0)->getValue());
    rotation->addValue(selectedEntity->getBrain()->getOutputNeurons().at(1)->getValue());
    mouth->addValue(selectedEntity->getBrain()->getOutputNeurons().at(2)->getValue());
    genitals->addValue(selectedEntity->getBrain()->getOutputNeurons().at(3)->getValue());


    grad0->addValue(0.0);
    grad1->addValue(1.0);
    gradm1->addValue(-1.0);

    energy->addValue(selectedEntity->getEnergyCenter()->getAvailableEnergy());
    mass->addValue(selectedEntity->getEntity()->getMass());
    maxEnergy->addValue(selectedEntity->getEnergyCenter()->getMaxMass());
    wasted->addValue(selectedEntity->getEnergyCenter()->getWastedEnergy());
}

void LifeScreen::drawGraphs(sf::RenderWindow *pWindow) {
    if (selectedEntity == nullptr) {
        return;
    }
    outputNeuronsGraph->draw(pWindow);
    bioGraph->draw(pWindow);
}



void LifeScreen::mouseScrolled(float delta, int mouseX, int mouseY) {
    Screen::mouseScrolled(delta, mouseX, mouseY);

    float deltaRatio;
    if (delta < 0) {
        camera->changeZoom(0.9f);
        deltaRatio = 1.1;
    } else if (delta > 0) {
        camera->changeZoom(1.1f);
        deltaRatio = 0.9;
    } else {
        return;
    }

    Point cameraCenter = camera->getCenter();
    Point mouseWorldCoordinates = camera->getWorldCoordinates({float(mouseX), float(mouseY)});

    float deltaX = cameraCenter.getX() - mouseWorldCoordinates.getX();
    float deltaY = cameraCenter.getY() - mouseWorldCoordinates.getY();

    float newX = mouseWorldCoordinates.getX() + (deltaX * (deltaRatio));
    float newY = mouseWorldCoordinates.getY() + (deltaY * (deltaRatio));

    Point newCameraCenter = {newX, newY};
    camera->setCenter(newCameraCenter);


    farmUi->mouseMoved(mouseWorldCoordinates, camera);
}
