//
// Created by Amalric Lombard de Buffières on 8/8/20.
//

#include <iostream>
#include <cmath>
#include "Graph.h"


const std::string &Graph::getName() const {
    return name;
}

void Graph::setName(const std::string &name) {
    Graph::name = name;
}

const std::vector<DataItemConnector> &Graph::getLines() const {
    return lines;
}

void Graph::setLines(const std::vector<DataItemConnector> &lines) {
    Graph::lines = lines;
}

Graph::Graph(const std::string &name, sf::Font *font) : name(name), font(font) {
    min = 10000000;
    max = -10000000;

    background = sf::RectangleShape();
    background.setFillColor(sf::Color(100, 100, 100, 255));
}

void Graph::setPosition(float xScreenRatio, float yScreenRatio, float widthScreenRatio, float heightScreenRatio) {
    this->xScreenRatio = xScreenRatio;
    this->yScreenRatio = yScreenRatio;
    this->widthScreenRatio = widthScreenRatio;
    this->heightScreenRatio = heightScreenRatio;
}


void Graph::addLine(DataItem * item, int displayMode, int red, int green, int blue) {
    DataItemConnector connector = DataItemConnector(item, displayMode, red, green, blue);
    lines.emplace_back(connector);

    int lineIndex = lines.size() - 1;

    sf::Color legendBackgroundColor = sf::Color(64, 64, 64, 255);
    sf::Color backgroundColor = sf::Color(55, 55, 55, 255);
    sf::Color textColor = sf::Color(red, green, blue, 255);

    int yButtons = 300;
    int xLine = (lineIndex * 210);

    sf::RectangleShape legendBackground = sf::RectangleShape(sf::Vector2f(200, 50));
    legendBackground.setPosition(xLine, yButtons);
    legendBackground.setFillColor(legendBackgroundColor);
    linesLegendBackgrounds.emplace_back(legendBackground);

    sf::Text text;
    text.setFont(*font);
    text.setString(item->getName());
    text.setFillColor(textColor);
    text.setCharacterSize(15);
    text.setPosition(xLine + 5, 0);
    linesLegendTexts.emplace_back(text);

    sf::Color legendButtonBackgroundColor = sf::Color(100, 100, 100, 255);

    Button * doNotShowButton = new Button("X", lineIndex, font, 5 + xLine, 0, 50, 20, legendButtonBackgroundColor, textColor);
    Button * showNormalButton = new Button("V", lineIndex, font, 5 + xLine + 55, 0, 50, 20, legendButtonBackgroundColor, textColor);
    Button * showAveragedButton = new Button("~", lineIndex, font,5 + xLine + 110, 0, 50, 20, legendButtonBackgroundColor, textColor);

    doNotShowButtons.emplace_back(doNotShowButton);
    showNormalButtons.emplace_back(showNormalButton);
    showAveragedButtons.emplace_back(showAveragedButton);

    setLegendsButtonColors();
}

void Graph::windowResized(float windowWidth, float windowHeight) {
    x = windowWidth * xScreenRatio;
    y = windowHeight * yScreenRatio;

    width = windowWidth * widthScreenRatio;
    height = windowHeight * heightScreenRatio;

    background.setPosition(x, y);
    background.setSize(sf::Vector2f(width, height));


    double buttonsPositionY = (0.1 * windowHeight) - 60;

    for (int it = 0; it < linesLegendBackgrounds.size(); it++) {
        linesLegendBackgrounds.at(it).setPosition(linesLegendBackgrounds.at(it).getPosition().x, buttonsPositionY);
        linesLegendTexts.at(it).setPosition(linesLegendTexts.at(it).getPosition().x, buttonsPositionY + 5);

        doNotShowButtons.at(it)->move(doNotShowButtons.at(it)->getX(), buttonsPositionY + 25);
        showNormalButtons.at(it)->move(showNormalButtons.at(it)->getX(), buttonsPositionY + 25);
        showAveragedButtons.at(it)->move(showAveragedButtons.at(it)->getX(), buttonsPositionY + 25);
    }
}

void Graph::getMinAndMaxValues() {
    min = 10000000.0;
    max = -10000000.0;

    for (int it = 0; it < lines.size(); it++) {
        if (lines.at(it).getDisplayMode() == 0) {
            continue;
        }

        if (lines.at(it).getMin() < min) {
            min = lines.at(it).getMin();
        }

        if (lines.at(it).getMax() > max) {
            max = lines.at(it).getMax();
        }
    }

    widthRatio = width / float(lines.at(0).getItem()->getCount());

    heightRatio = 1;

    heightRatio = 0;

    if (max - min != 0) {
        heightRatio = height / abs(max - min);
    }
}


void Graph::drawStat(sf::RenderWindow *window, DataItemConnector line) {
    if (line.getDisplayMode() == 0) {
        return;
    }

    int currentLineItemCount = line.getItem()->getCount();

    sf::VertexArray lineVertexes = sf::VertexArray(sf::LineStrip, currentLineItemCount);
    sf::Color lineColor = sf::Color(line.getRed(), line.getGreen(), line.getBlue(), 255);

    for (int it = 0; it < currentLineItemCount; it++) {
        double calculatedX = x + (it * widthRatio);
        double calculatedY = (y + height) - ((line.getValueForTick(it) - min) * heightRatio);

        lineVertexes[it].position = sf::Vector2f(calculatedX, calculatedY);
        lineVertexes[it].color = lineColor;
    }

    window->draw(lineVertexes);
    lineVertexes.clear();
}

void Graph::draw(sf::RenderWindow *window) {
    getMinAndMaxValues();

    window->draw(background);
    for (int it = 0; it < lines.size(); it++) {
        window->draw(linesLegendBackgrounds.at(it));
        window->draw(linesLegendTexts.at(it));
        doNotShowButtons.at(it)->draw(window);
        showAveragedButtons.at(it)->draw(window);
        showNormalButtons.at(it)->draw(window);
        drawStat(window, lines.at(it));
    }
}

void Graph::mouseClicked(int x, int y) {
    for (int it = 0; it < doNotShowButtons.size(); it++) {
        if (doNotShowButtons.at(it)->clicked(x, y)) {
            lines.at(it).setDisplayMode(0);
        }
        if (showNormalButtons.at(it)->clicked(x, y)) {
            lines.at(it).setDisplayMode(1);

        }
        if (showAveragedButtons.at(it)->clicked(x, y)) {
            lines.at(it).setDisplayMode(2);
        }
    }

    setLegendsButtonColors();
}

void Graph::setLegendsButtonColors() {
    sf::Color notSelectedBackgroundColor = sf::Color(33, 33, 33, 255);
    sf::Color selectedBackgroundColor = sf::Color(100, 100, 100, 255);

    for (int it = 0; it < lines.size(); it++) {
        doNotShowButtons.at(it)->changeBackgroundColor(notSelectedBackgroundColor);
        showNormalButtons.at(it)->changeBackgroundColor(notSelectedBackgroundColor);
        showAveragedButtons.at(it)->changeBackgroundColor(notSelectedBackgroundColor);

        switch (lines.at(it).getDisplayMode()) {
            case 0:
                doNotShowButtons.at(it)->changeBackgroundColor(selectedBackgroundColor);
                break;
            case 1:
                showNormalButtons.at(it)->changeBackgroundColor(selectedBackgroundColor);
                break;
            case 2:
                showAveragedButtons.at(it)->changeBackgroundColor(selectedBackgroundColor);
                break;
        }
    }
}
