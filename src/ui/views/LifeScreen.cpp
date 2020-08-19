//
// Created by Amalric Lombard de Buffières on 8/19/20.
//

#include "LifeScreen.h"

LifeScreen::LifeScreen(Farm *farm, sf::Font *font) : Screen(farm, font), camera(nullptr), informationToShow("BRAIN") {}

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

    Button genomeButton = Button("Brain", 1, font, 0, 0.95 * windowHeight, 100, 50, backgroundColor, textColor);
    Button brainButton = Button("Genome", 2, font, 110, 0.95 * windowHeight, 100, 50, backgroundColor, textColor);

    buttons.emplace_back(brainButton);
    buttons.emplace_back(genomeButton);

}

Camera *LifeScreen::open() {
    return camera;
}

void LifeScreen::draw(sf::RenderWindow *window) {
    window->draw(mapBackground);


    for (int it = 0; it < buttons.size(); it++) {
        buttons.at(it).draw(window);
    }

    if (selectedLife == nullptr || camera == nullptr) {
        return ;
    }


    camera->setCenter(selectedLife->getEntity()->getPosition());


    if (informationToShow == "BRAIN") {
        if (brainUi != nullptr) {
            brainUi->draw(window);
        }
    }

    if (informationToShow == "GENOME") {
        drawGenome(window);
    }



}

void LifeScreen::drawGenome(sf::RenderWindow *window) {
    for (int it = 0; it < genomeTexts.size(); it++) {
        window->draw(genomeTexts.at(it));
    }
}


void LifeScreen::updateSelectedLife(Life * life) {
    selectedLife = life;

    if (selectedLife == nullptr) {
        brainUi = nullptr;
    } else {
        brainUi = new BrainUI(life->getBrain(), windowWidth * 0.55, 0.1 * windowHeight, 0.40 * windowWidth,  0.8 * windowHeight, font);
    }

    loadSelectedGenome();

}

void LifeScreen::loadSelectedGenome() {
    genomeTexts.clear();


    std::vector<Evolution *> genome = farm->getNursery()->getEvolutionLibrary()->getGenomeFor(selectedLife->getEntity()->getId());
    sf::Color textColor = sf::Color(200, 200, 200, 255);

    for (int it = 0; it < genome.size(); it++) {
        sf::Text currentText;
        currentText.setString(genome.at(it)->describe());
        currentText.setFont(*font);
        currentText.setFillColor(textColor);
        currentText.setCharacterSize(20);

        int x = 0.6 * windowWidth;
        int y = (0.1 * windowHeight) + (it * 30);

        currentText.setPosition(x, y);

        genomeTexts.emplace_back(currentText);
    }

}

void LifeScreen::onWindowResize(int width, int height) {
    windowWidth = width;
    windowHeight = height;

    camera->setTopLeft(Point(0.05 * windowWidth, 0.1f * height));
    camera->setWidth(0.45 * width);
    camera->setHeight(0.8f * height);

    mapBackground.setPosition(0.05 * windowWidth, 0.1 * height);
    mapBackground.setSize(sf::Vector2f(0.45 * width, 0.8 * height));


    for (int it = 0; it < buttons.size(); it++) {
        buttons.at(it).move(buttons.at(it).getX(), height - 100);
    }

}

void LifeScreen::mouseMoved(int x, int y) {

}

void LifeScreen::mouseClicked(int x, int y) {
    Screen::mouseClicked(x, y);


    for (int it = 0; it < buttons.size(); it++) {
        if (buttons.at(it).clicked(x, y)) {
            handleButtonClick(buttons.at(it).getId());
        }
    }

}

void LifeScreen::handleButtonClick(int id) {
    switch (id) {
        case 1:
            informationToShow = "BRAIN";
            break;
        case 2:
            informationToShow = "GENOME";
            break;
    }
}