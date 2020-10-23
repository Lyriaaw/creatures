//
// Created by Amalric Lombard de Buffières on 8/8/20.
//

#include <iostream>
#include <cmath>
#include "Graph.h"
#include "../elements/GlobalFont.h"
#include "../elements/UiButton.h"


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

Graph::Graph(const std::string &name, float xRatio, float yRatio, float widthRatio, float heightRatio): UiComponent(xRatio, yRatio, widthRatio, heightRatio), name(name) {
    min = 10000000;
    max = -10000000;

    background = sf::RectangleShape();
    background.setFillColor(sf::Color(100, 100, 100, 255));

    hoveredValuesBackground = new UiBackground(0, 0, 0, 0);
    hoveredValuesBackground->setFillColor(sf::Color(60, 60, 60, 200));
    uiComponents.emplace_back(hoveredValuesBackground);

    hoveredTickMarker = new UiBackground(0, 0, 0, 0);
    hoveredTickMarker->setFillColor(sf::Color(0, 0, 0, 200));
    uiComponents.emplace_back(hoveredTickMarker);



    UiLabel * hoveredInfoLabel = new UiLabel(0, 0, 0, 0);
    hoveredInfoLabel->setText("ticks");
    hoveredInfoLabel->setCharacterSize(15);
    hoveredInfoLabel->setFillColor(sf::Color(255, 255, 255, 255));
    uiComponents.emplace_back(hoveredInfoLabel);
    hoveredInfoValues.emplace_back(hoveredInfoLabel);

}

void Graph::addLine(DataItem * item, int displayMode, int red, int green, int blue) {
    DataItemConnector connector = DataItemConnector(item, displayMode, red, green, blue);
    lines.emplace_back(connector);

    int lineIndex = lines.size() - 1;

    sf::Color legendBackgroundColor = sf::Color(75, 75, 75, 255);
    sf::Color backgroundColor = sf::Color(55, 55, 55, 255);
    sf::Color textColor = sf::Color(red, green, blue, 255);


    double xLine = xRatio + (lineIndex * 0.08);
    double currentHeightRatio = heightRatio;
    double yButtons = double(yRatio) + double(currentHeightRatio) + 0.01;

    UiBackground * legendBackground = new UiBackground(xLine, yButtons, 0.07, 0.04);
    legendBackground->setFillColor(legendBackgroundColor);
    uiComponents.emplace_back(legendBackground);

    UiLabel * legendLabel = new UiLabel(xLine + 0.001, yButtons + 0.001, 0, 0);
    legendLabel->setCharacterSize(15);
    legendLabel->setText(item->getName());
    legendLabel->setFillColor(textColor);
    uiComponents.emplace_back(legendLabel);

    UiButton * doNotShowButton = new UiButton(lineIndex, "X", xLine, yButtons + 0.02, 0.02, 0.02);
    doNotShowButton->setOnClick([&](int it) {
        lines.at(it).setDisplayMode(0);
        setLegendsButtonColors();
    });
    uiComponents.emplace_back(doNotShowButton);
    doNotShowButtons.emplace_back(doNotShowButton);

    UiButton * showNormal = new UiButton(lineIndex, "V", xLine + 0.021, yButtons + 0.02, 0.02, 0.02);
    showNormal->setOnClick([&](int it) {
        lines.at(it).setDisplayMode(1);
        setLegendsButtonColors();
    });
    uiComponents.emplace_back(showNormal);
    showNormalButtons.emplace_back(showNormal);

    UiButton * showAveraged = new UiButton(lineIndex, "~", xLine + 0.042, yButtons + 0.02, 0.02, 0.02);
    showAveraged->setOnClick([&](int it) {
        lines.at(it).setDisplayMode(2);
        setLegendsButtonColors();
    });
    uiComponents.emplace_back(showAveraged);
    showAveragedButtons.emplace_back(showAveraged);

    setLegendsButtonColors();

    UiLabel * hoveredInfoLabel = new UiLabel(0, 0, 0, 0);
    hoveredInfoLabel->setText(item->getName());
    hoveredInfoLabel->setCharacterSize(15);
    hoveredInfoLabel->setFillColor(textColor);
    uiComponents.emplace_back(hoveredInfoLabel);
    hoveredInfoValues.emplace_back(hoveredInfoLabel);

}

void Graph::setLegendsButtonColors() {
    sf::Color notSelectedBackgroundColor = sf::Color(33, 33, 33, 255);
    sf::Color selectedBackgroundColor = sf::Color(100, 100, 100, 255);

    for (int it = 0; it < lines.size(); it++) {
        doNotShowButtons.at(it)->setFillColor(notSelectedBackgroundColor);
        showNormalButtons.at(it)->setFillColor(notSelectedBackgroundColor);
        showAveragedButtons.at(it)->setFillColor(notSelectedBackgroundColor);

        switch (lines.at(it).getDisplayMode()) {
            case 0:
                doNotShowButtons.at(it)->setFillColor(selectedBackgroundColor);
                break;
            case 1:
                showNormalButtons.at(it)->setFillColor(selectedBackgroundColor);
                break;
            case 2:
                showAveragedButtons.at(it)->setFillColor(selectedBackgroundColor);
                break;
        }
    }
}

void Graph::updateHoveredTickInformation() {
    hoveredTick = lastMouseX / tickWidthRatio;

    if (graphControlCenter->getMode() == "last") {
        hoveredTick = lines.at(0).getItem()->getCount() - graphControlCenter->getLastTickFrame() + hoveredTick;
    } else if (graphControlCenter->getMode() == "fixed") {
        hoveredTick = graphControlCenter->getFromTick() + hoveredTick;
    }

    for (int it = 0; it < hoveredInfoValues.size(); it++) {
        if (it == 0) {
            hoveredInfoValues.at(it)->setText("Tick: " + std::to_string(hoveredTick));
            continue;
        }

        DataItemConnector currentLine = lines.at(it - 1);
        DataItem * currentItem = currentLine.getItem();

        double currentValue = currentLine.getDisplayMode() == 2 ? currentItem->getAveragedValueForTick(hoveredTick) : currentItem->getValueForTick(hoveredTick);


        hoveredInfoValues.at(it)->setText(currentItem->getName() + " : " + std::to_string(currentValue));
    }
}



void Graph::windowResized(float windowWidth, float windowHeight) {
    UiComponent::onWindowResized(windowWidth, windowHeight);

    for (int it = 0; it < uiComponents.size(); it++) {
        uiComponents.at(it)->onWindowResized(windowWidth, windowHeight);
    }

    background.setPosition(x, y);
    background.setSize(sf::Vector2f(width, height));

}

void Graph::getMinAndMaxValues() {
    min = 100000000000.0;
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

//    int itemCount = std::max(lines.at(0).getItem()->getCount(), lines.at(1).getItem()->getCount());
    int itemCount = lines.at(0).getItem()->getCount();

    if (graphControlCenter->getMode() == "last") {
        itemCount = graphControlCenter->getLastTickFrame();
    } else if (graphControlCenter->getMode() == "fixed") {
        itemCount = graphControlCenter->getToTick() - graphControlCenter->getFromTick();
    }



    tickWidthRatio = width / float(itemCount);

    tickHeightRatio = 1;

    if (max - min != 0) {
        tickHeightRatio = height / abs(max - min);
    }
}


void Graph::drawStat(sf::RenderWindow *window, DataItemConnector line) {
    if (line.getDisplayMode() == 0) {
        return;
    }

    int itemCount = line.getItem()->getCount();
//    int itemCount = std::max(lines.at(0).getItem()->getCount(), lines.at(1).getItem()->getCount());


    int currentItemcount = itemCount;
    if (graphControlCenter->getMode() == "last") {
        currentItemcount = graphControlCenter->getLastTickFrame();
    } else if (graphControlCenter->getMode() == "fixed") {
        currentItemcount = graphControlCenter->getToTick() - graphControlCenter->getFromTick();
    }

    sf::VertexArray lineVertexes = sf::VertexArray(sf::LineStrip, currentItemcount);
    sf::Color lineColor = sf::Color(line.getRed(), line.getGreen(), line.getBlue(), 255);



    for (int it = 0; it < currentItemcount; it++) {
        int currentTick = it;

        if (graphControlCenter->getMode() == "last") {
            currentTick = itemCount - graphControlCenter->getLastTickFrame() + it;
        } else if (graphControlCenter->getMode() == "fixed") {
            currentTick = graphControlCenter->getFromTick() + it;
        }


        double currentValue = line.getValueForTick(currentTick);
        double calculatedX = x + (it * tickWidthRatio);
        double calculatedY = (y + height) - ((currentValue - min) * tickHeightRatio);

        lineVertexes[it].position = sf::Vector2f(calculatedX, calculatedY);
        lineVertexes[it].color = lineColor;
    }

    window->draw(lineVertexes);
    lineVertexes.clear();
}



void Graph::draw(sf::RenderWindow *window) {
    getMinAndMaxValues();

    updateHoveredTickInformation();
    window->draw(background);


    for (int it = 0; it < lines.size(); it++) {
        drawStat(window, lines.at(it));
    }


    for (int it = 0; it < uiComponents.size(); it++) {
        uiComponents.at(it)->draw(window);
    }
}


void Graph::mouseClicked(int x, int y) {
    UiComponent::mouseClicked(x, y);

    for (int it = 0; it < uiComponents.size(); it++) {
        uiComponents.at(it)->mouseClicked(x, y);
    }

    setLegendsButtonColors();
}



void Graph::mouseMoved(int mouseX, int mouseY, int previousX, int previousY) {
    UiComponent::mouseMoved(mouseX, mouseY, previousX, previousY);
}

void Graph::mouseHovering(int mouseX, int mouseY) {
    int textX = std::min(double(width - 500), double(mouseX));
    lastMouseX = mouseX;

    hoveredValuesBackground->setScreenPlacementInformation(textX, mouseY, 500, 500);
    hoveredTickMarker->setScreenPlacementInformation(mouseX, y, 1, height);

    for (int it = 0; it < hoveredInfoValues.size(); it++) {
        hoveredInfoValues.at(it)->setScreenPlacementInformation(textX + 20, mouseY + (30 * it) + 30, 0, 0);
    }

    updateHoveredTickInformation();
}

void Graph::mouseLeave() {
    hoveredValuesBackground->setScreenPlacementInformation(0, 0, 0, 0);
    hoveredTickMarker->setScreenPlacementInformation(0, 0, 0, 0);

    for (int it = 0; it < hoveredInfoValues.size(); it++) {
        hoveredInfoValues.at(it)->setScreenPlacementInformation(-100, -100, 0, 0);
    }
}


void Graph::mouseClickedInside(int mouseX, int mouseY) {

}

void Graph::resized() {

}

void Graph::mouseEnter() {

}

void Graph::setGraphControlCenter(GraphControlCenter *graphControlCenter) {
    Graph::graphControlCenter = graphControlCenter;
}


