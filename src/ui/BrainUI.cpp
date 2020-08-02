//
// Created by Amalric Lombard de Buffières on 8/2/20.
//

#include "BrainUI.h"
#include "colors/RGBColor.h"


BrainUI::BrainUI(Brain *brain, float x, float y, float width, float height) : brain(brain), x(x), y(y), width(width), height(height) {
    initialize();
}

void BrainUI::initialize() {



    background = sf::RectangleShape(sf::Vector2f(width, height));
    background.setPosition(x, y);
    background.setFillColor(sf::Color(100, 100, 100, 255));



    float networkWidth = 20.f;
    float networkHeight = 10.f;

    for (int it = 0; it < brain->getNeurons().size(); it++) {
        if (networkHeight < brain->getNeurons().at(it)->getY()) {
            networkHeight = brain->getNeurons().at(it)->getY();
        }
    }

    float margin = 20.f;
    float circleRadius = 10.f;

    float networkWidthRatio  = width / (networkWidth);
    float networkHeightRatio = height / float(networkHeight + margin);


    for (int it = 0; it < brain->getNeurons().size(); it++) {
        Neuron * currentNeuron = brain->getNeurons().at(it);

        sf::CircleShape circle(circleRadius);

        RGBColor outlineColor = RGBColor(currentNeuron->getHueOutline(), 1.f, currentNeuron->getBrightnessOutline());
        circle.setOutlineColor(sf::Color(outlineColor.getRed(), outlineColor.getGreen(), outlineColor.getBlue(), 255));
        circle.setOutlineThickness(1.f);

        circle.setPosition(x + (currentNeuron->getX() * networkWidthRatio) - circleRadius + ((margin / 2.f) * networkWidthRatio), y + (currentNeuron->getY() * networkHeightRatio) - circleRadius + ((margin / 2.f) * networkHeightRatio));
        neurons.emplace_back(circle);
    }


}

void BrainUI::draw(sf::RenderWindow *window) {
    window->draw(background);

    for (int it = 0; it < neurons.size(); it++) {
        Neuron * currentNeuron = brain->getNeurons().at(it);
        float currentNeuronValue = currentNeuron->getValue();

        neurons.at(it).setFillColor(sf::Color(currentNeuronValue * 255, currentNeuronValue * 255, currentNeuronValue * 255, 255));

        window->draw(neurons.at(it));
    }
}


