//
// Created by Amalric Lombard de Buffières on 8/8/20.
//

#include <iostream>
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

Graph::Graph(const std::string &name) : name(name) {
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

void Graph::windowResized(float windowWidth, float windowHeight) {
    x = windowWidth * xScreenRatio;
    y = windowHeight * yScreenRatio;

    width = windowWidth * widthScreenRatio;
    height = windowHeight * heightScreenRatio;

    background.setPosition(x, y);
    background.setSize(sf::Vector2f(width, height));
}


void Graph::addLine(DataItem * item, int red, int green, int blue) {
    DataItemConnector connector = DataItemConnector(item, red, green, blue);
    lines.emplace_back(connector);
}

void Graph::getMinAndMaxValues() {
    for (int it = 0; it < lines.size(); it++) {
        if (lines.at(it).getItem()->getAveraged()->getMin() < min) {
            min = lines.at(it).getItem()->getAveraged()->getMin();
        }

        if (lines.at(it).getItem()->getAveraged()->getMax() > max) {
            max = lines.at(it).getItem()->getAveraged()->getMax();
        }
    }

    widthRatio = width / float(lines.at(0).getItem()->getCount());

    heightRatio = 1;

    heightRatio = 0;

    if (max != 0) {
        heightRatio = height / max;
    }
}


void Graph::drawStat(sf::RenderWindow *window, DataItemConnector line) {
    sf::VertexArray lineVertexes = sf::VertexArray(sf::LineStrip, line.getItem()->getCount());
    sf::Color lineColor = sf::Color(line.getRed(), line.getGreen(), line.getBlue(), 255);

    for (int it = 0; it < line.getItem()->getCount(); it++) {
        double calculatedX = x + (it * widthRatio);
        double calculatedY = (y + height) - (line.getItem()->getAveragedValueForTick(it) * heightRatio);

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
        drawStat(window, lines.at(it));
    }
}
