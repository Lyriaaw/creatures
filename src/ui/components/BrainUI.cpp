//
// Created by Amalric Lombard de Buffières on 8/2/20.
//

#include "BrainUI.h"
#include "../colors/RGBColor.h"
#include "../elements/GlobalFont.h"


BrainUI::BrainUI(Brain *brain, float x, float y, float width, float height, sf::Font * font, float windowWidth, float windowHeight) : brain(brain), x(x), y(y), width(width), height(height), font(font), windowWidth(windowWidth), windowHeight(windowHeight), hoveredNeuron(
        nullptr) {
    initialize();
}



void BrainUI::initialize() {

    background = sf::RectangleShape(sf::Vector2f(width, height));
    background.setPosition(x, y);
    background.setFillColor(sf::Color(128, 128, 128, 255));

    calculatePlacementValues();
    initializeLinks();
    initializeNeurons();



}

void BrainUI::initializeLinks() {
    links = sf::VertexArray(sf::Lines, brain->getLinks().size() * 2);
    for (int it = 0; it < brain->getLinks().size(); it++) {
        Link * currentLink = brain->getLinks().at(it);

        if (hoveredNeuron != nullptr) {
            if (hoveredNeuron->getGenerationNumber() != currentLink->getInput()->getGenerationNumber() && hoveredNeuron->getGenerationNumber() != currentLink->getOutput()->getGenerationNumber()) {
                continue;
            }
        }

        float inputNeuronScreenX = x + (currentLink->getInput()->getX() * networkWidthRatio) + ((margin / 2.f) * networkWidthRatio);
        float inputNeuronScreenY = y + (currentLink->getInput()->getY() * networkHeightRatio) + ((margin / 2.f) * networkHeightRatio);

        float outputNeuronScreenX = x + (currentLink->getOutput()->getX() * networkWidthRatio) + ((margin / 2.f) * networkWidthRatio);
        float outputNeuronScreenY = y + (currentLink->getOutput()->getY() * networkHeightRatio) + ((margin / 2.f) * networkHeightRatio);

        links[(2 * it)].position = sf::Vector2f(inputNeuronScreenX, inputNeuronScreenY);
        links[(2 * it) + 1].position = sf::Vector2f(outputNeuronScreenX, outputNeuronScreenY);

        float colorRatio = (currentLink->getWeight() + 1) / 2.f;
        links[(2 * it)].color = sf::Color(255 * colorRatio, 255 * colorRatio, 255 * colorRatio, 255);
        links[(2 * it) + 1].color = sf::Color(255 * colorRatio, 255 * colorRatio, 255 * colorRatio, 255);
    }
}

void BrainUI::initializeNeurons() {
    for (int it = 0; it < brain->getNeurons().size(); it++) {
        Neuron * currentNeuron = brain->getNeurons().at(it);

        sf::CircleShape circle(circleRadius);

        RGBColor outlineColor = RGBColor(currentNeuron->getHueOutline(), 1.f, currentNeuron->getBrightnessOutline());
        circle.setOutlineColor(sf::Color(outlineColor.getRed(), outlineColor.getGreen(), outlineColor.getBlue(), 255));
        circle.setOutlineThickness(1.f);

        float neuronScreenX = x + (currentNeuron->getX() * networkWidthRatio) - circleRadius + ((margin / 2.f) * networkWidthRatio);
        float neuronScreenY = y + (currentNeuron->getY() * networkHeightRatio) - circleRadius + ((margin / 2.f) * networkHeightRatio);

        circle.setPosition(neuronScreenX, neuronScreenY);
        neurons.emplace_back(circle);

        sf::Text text;
        text.setFont(*GlobalFont::MainFont);
        text.setString(currentNeuron->getName() + " (" + std::to_string(currentNeuron->getGenerationNumber()) + ")");
        text.setFillColor(sf::Color(255, 255, 255));
        text.setCharacterSize(18);
        text.setPosition(neuronScreenX - text.getGlobalBounds().width - 20, neuronScreenY - (text.getGlobalBounds().height / 4.f));
        neuronNames.emplace_back(text);
    }
}


void BrainUI::draw(sf::RenderWindow *window) {

    window->draw(background);

    window->draw(links);

    for (int it = 0; it < neurons.size(); it++) {
        Neuron * currentNeuron = brain->getNeurons().at(it);
        float currentNeuronValue = (currentNeuron->getValue() / 2.f) + 0.5;

        neurons.at(it).setFillColor(sf::Color(currentNeuronValue * 255, currentNeuronValue * 255, currentNeuronValue * 255, 255));
        window->draw(neurons.at(it));
    }



    for (int it = 0; it < neuronNames.size(); it++) {
        window->draw(neuronNames.at(it));
    }
}

bool BrainUI::mouseClicked(int mouseX, int mouseY) {

    if (mouseX < x) {
        return false;
    }

    if (hoveredNeuron == nullptr) {
        return false;
    }

    std::cout << "Neuron: " << hoveredNeuron->getGenerationNumber() << std::endl;

    for (int it = 0; it < brain->getLinks().size(); it++) {
        Link * currentLink = brain->getLinks().at(it);

        if (hoveredNeuron != nullptr) {
            if (hoveredNeuron->getGenerationNumber() != currentLink->getInput()->getGenerationNumber() && hoveredNeuron->getGenerationNumber() != currentLink->getOutput()->getGenerationNumber()) {
                continue;
            }
        }

        std::cout << "Link: " << currentLink->getInput()->getGenerationNumber() << " => " << currentLink->getOutput()->getGenerationNumber() << " ( " << currentLink->getWeight() << " )" << std::endl;

    }

    std::cout << std::endl;
    return true;
}

void BrainUI::calculatePlacementValues() {

    networkWidth = 20.f;
    networkHeight = 10.f;

    for (int it = 0; it < brain->getNeurons().size(); it++) {
        if (networkHeight < brain->getNeurons().at(it)->getY()) {
            networkHeight = brain->getNeurons().at(it)->getY();
        }
        if (networkWidth < brain->getNeurons().at(it)->getX()) {
            networkWidth = brain->getNeurons().at(it)->getX();
        }

    }

    margin = 20.f;
    circleRadius = 10.f;

    networkWidthRatio  = width / (networkWidth + margin);
    networkHeightRatio = height / float(networkHeight + margin);
}

bool BrainUI::mouseMoved(int mouseX, int mouseY) {


    bool found = false;
    for (int it = 0; it < brain->getNeurons().size(); it++) {
        Neuron * currentNeuron = brain->getNeurons().at(it);

        float neuronScreenX = x + (currentNeuron->getX() * networkWidthRatio) - circleRadius + ((margin / 2.f) * networkWidthRatio);
        float neuronScreenY = y + (currentNeuron->getY() * networkHeightRatio) - circleRadius + ((margin / 2.f) * networkHeightRatio);

        float absX = abs(neuronScreenX - mouseX);
        float absY = abs(neuronScreenY - mouseY);


        if (absX < 2 * circleRadius && absY < 2 * circleRadius) {
            hoveredNeuron = currentNeuron;
            found = true;
        }
    }

    if (!found) {
        hoveredNeuron = nullptr;
    }

    initializeLinks();


    return true;
}


