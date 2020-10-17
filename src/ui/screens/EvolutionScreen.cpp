//
// Created by Amalric Lombard de Buffières on 10/17/20.
//

#include "EvolutionScreen.h"

EvolutionScreen::EvolutionScreen(FarmUI * farmUi) : Screen(farmUi), lastEvolutionsCount(0) {
    windowWidth = 0;
    windowHeight = 0;
}

int EvolutionScreen::getId() {
    return 6;
}

void EvolutionScreen::loadTexts() {
    currentGenerationNumberText = new sf::Text();
    currentGenerationNumberText->setFont(*font);
    currentGenerationNumberText->setString("Tick");
    currentGenerationNumberText->setFillColor(sf::Color(255, 255, 255, 255));
    currentGenerationNumberText->setCharacterSize(30);

}

void EvolutionScreen::loadButtons(sf::Font *font) {
    sf::Color backgroundColor = sf::Color(55, 55, 55, 255);
    sf::Color textColor = sf::Color(195, 195, 195, 255);
}

void EvolutionScreen::placeElements() {
    currentGenerationNumberText->setPosition(0.01 * windowWidth, 0.05 * windowHeight);

    for (int it = 0; it < evolutionButtons.size(); it++) {
        double buttonY = 0.05 * windowHeight + (it * 50);

        evolutionButtons.at(it)->move(0.01 * windowWidth, buttonY);
    }
}

void EvolutionScreen::init() {
    loadTexts();
    loadButtons(font);
}


void EvolutionScreen::updateEvolutions(std::map<int, Evolution *> newAllEvolutions) {
    if (allEvolutions.size() == newAllEvolutions.size()) {
        return;
    }
    allEvolutions.clear();
    allEvolutions.insert(newAllEvolutions.begin(), newAllEvolutions.end());

    sf::Color backgroundColor = sf::Color(50, 50, 50, 255);
    sf::Color textColor = sf::Color(205, 205, 205, 255);

    int evolutionCount = 0;
    int displayedEvolutionCount = 0;
    std::map<int, Evolution *>::iterator it;
    for (it = allEvolutions.begin(); it != allEvolutions.end(); it++) {
        evolutionCount++;
        Evolution * currentEvolution = it->second;
        if (currentEvolution->getName() == "Link") {
            continue;
        }

        displayedEvolutionCount++;

        if (lastEvolutionsCount > currentEvolution->getGenerationNumber()) {
            continue;
        }



        std::string buttonText = "#" + std::to_string(currentEvolution->getGenerationNumber()) + " " + currentEvolution->getName();

        double buttonY = 0.05 * windowHeight + (displayedEvolutionCount * 55);
        Button * button = new Button(buttonText, currentEvolution->getGenerationNumber(), font, 0.01 * windowWidth, buttonY, 300, 50, backgroundColor, textColor);
        evolutionButtons.emplace_back(button);

    }


    lastEvolutionsCount = allEvolutions.size();
}

void EvolutionScreen::updateTexts() {
    std::map<int, Evolution *> newAllEvolutions = farmUi->getFarm()->getNursery()->getEvolutionLibrary().getAllEvolutions();
    updateEvolutions(newAllEvolutions);

    currentGenerationNumberText->setString("Evolutions: " + std::to_string(lastEvolutionsCount));
}

void EvolutionScreen::draw(sf::RenderWindow *window) {
    updateTexts();
    window->draw(*currentGenerationNumberText);

    for (int it = 0; it < evolutionButtons.size(); it++) {
        evolutionButtons.at(it)->draw(window);
    }
}

void EvolutionScreen::updateSelectedCreature() {

}

void EvolutionScreen::onWindowResize(int width, int height) {
    windowWidth = width;
    windowHeight = height;
    placeElements();
}


void EvolutionScreen::clickedOnButton(int id) {
}