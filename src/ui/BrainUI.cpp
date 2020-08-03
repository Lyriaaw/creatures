//
// Created by Amalric Lombard de Buffières on 8/2/20.
//

#include "BrainUI.h"
#include "colors/RGBColor.h"


BrainUI::BrainUI(Brain *brain, float x, float y, float width, float height, sf::Font * font) : brain(brain), x(x), y(y), width(width), height(height) {
    initialize(font);
}

void BrainUI::initialize(sf::Font * font) {



    background = sf::RectangleShape(sf::Vector2f(width, height));
    background.setPosition(x, y);
    background.setFillColor(sf::Color(128, 128, 128, 255));



    float networkWidth = 20.f;
    float networkHeight = 10.f;

    for (int it = 0; it < brain->getNeurons().size(); it++) {
        if (networkHeight < brain->getNeurons().at(it)->getY()) {
            networkHeight = brain->getNeurons().at(it)->getY();
        }
        if (networkWidth < brain->getNeurons().at(it)->getX()) {
            networkWidth = brain->getNeurons().at(it)->getX();
        }

    }

    float margin = 20.f;
    float circleRadius = 10.f;

    float networkWidthRatio  = width / (networkWidth + margin);
    float networkHeightRatio = height / float(networkHeight + margin);

    links = sf::VertexArray(sf::Lines, brain->getLinks().size() * 2);
    for (int it = 0; it < brain->getLinks().size(); it++) {
        Link * currentLink = brain->getLinks().at(it);

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
        text.setFont(*font);
        text.setString(currentNeuron->getName());
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
        float currentNeuronValue = currentNeuron->getValue();

        neurons.at(it).setFillColor(sf::Color(currentNeuronValue * 255, currentNeuronValue * 255, currentNeuronValue * 255, 255));

        window->draw(neurons.at(it));
    }



    for (int it = 0; it < neuronNames.size(); it++) {
        window->draw(neuronNames.at(it));
    }


}


