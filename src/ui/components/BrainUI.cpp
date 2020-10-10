//
// Created by Amalric Lombard de Buffières on 8/2/20.
//

#include "BrainUI.h"
#include "../colors/RGBColor.h"


BrainUI::BrainUI(Brain *brain, float x, float y, float width, float height, sf::Font * font, float windowWidth, float windowHeight) : brain(brain), x(x), y(y), width(width), height(height), selectedNeuron(nullptr), graph(nullptr), font(font), windowWidth(windowWidth), windowHeight(windowHeight) {
    initialize();
}

void BrainUI::initialize() {

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
        text.setString(currentNeuron->getName() + " (" + std::to_string(currentNeuron->getGenerationNumber()) + ")");
        text.setFillColor(sf::Color(255, 255, 255));
        text.setCharacterSize(18);
        text.setPosition(neuronScreenX - text.getGlobalBounds().width - 20, neuronScreenY - (text.getGlobalBounds().height / 4.f));
        neuronNames.emplace_back(text);
    }

    movement = new DataItem("movement", true);
    rotation = new DataItem("rotation", true);
    mouth = new DataItem("mouth", true);
    genitals = new DataItem("genitals", true);
    grad0 = new DataItem("0", true);
    grad1 = new DataItem("1", true);
    gradm1 = new DataItem("-1", true);

    graph = new Graph("Population", font);
    graph->setPosition(0.8f, 0.05f, 0.2f, 0.1f);

    graph->addLine(movement, 1, 255, 255, 255);
    graph->addLine(rotation, 1, 0, 0, 0);
    graph->addLine(mouth, 1, 0, 255, 0);
    graph->addLine(genitals, 1, 0, 0, 255);

    graph->addLine(grad0, 1, 0, 0, 0);
    graph->addLine(grad1, 1, 0, 0, 0);
    graph->addLine(gradm1, 1, 0, 0, 0);
    graph->windowResized(windowWidth, windowHeight);


}

void BrainUI::updateAndDrawSelectedNeuron(sf::RenderWindow *window) {
    movement->addValue(brain->getOutputNeurons().at(0)->getValue());
    rotation->addValue(brain->getOutputNeurons().at(1)->getValue());
    mouth->addValue(brain->getOutputNeurons().at(2)->getValue());
    genitals->addValue(brain->getOutputNeurons().at(3)->getValue());


    grad0->addValue(0.0);
    grad1->addValue(1.0);
    gradm1->addValue(-1.0);

    graph->draw(window);

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

    updateAndDrawSelectedNeuron(window);

}

bool BrainUI::mouseClicked(int mouseX, int mouseY) {

    if (mouseX < x) {
        return false;
    }

    return true;
}


