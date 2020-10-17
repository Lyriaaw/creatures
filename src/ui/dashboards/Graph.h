//
// Created by Amalric Lombard de Buffières on 8/8/20.
//

#ifndef CREATURES_GRAPH_H
#define CREATURES_GRAPH_H


#include <string>
#include <SFML/Graphics.hpp>
#include "DataItemConnector.h"
#include "../components/Button.h"
#include "../elements/UiComponent.h"
#include "../elements/UiLabel.h"
#include "../elements/UiBackground.h"

class Graph: public UiComponent {
protected:
    std::string name;
    std::vector<DataItemConnector> lines;

    std::vector<UiComponent *> uiComponents;

    std::vector<UiBackground *> labelsBackgrounds;
    std::vector<UiLabel *> labels;

    int mouseX, mouseY;

    double min, max;

    double tickWidthRatio, tickHeightRatio;

    sf::RectangleShape background;

    sf::RectangleShape hoverInfoBackground;
    std::vector<sf::Text *> hoveredInfoTexts;
    sf::Text * hoveredTickText;
    sf::RectangleShape hoveredTickBar;

    std::vector<sf::RectangleShape> linesLegendBackgrounds;
    std::vector<sf::Text> linesLegendTexts;

    std::vector<Button *> doNotShowButtons;
    std::vector<Button *> showNormalButtons;
    std::vector<Button *> showAveragedButtons;

public:
    Graph(const std::string &name, float xRatio, float yRatio, float widthRatio, float heightRatio);

    const std::string &getName() const;

    void setName(const std::string &name);

    const std::vector<DataItemConnector> &getLines() const;

    void setLines(const std::vector<DataItemConnector> &lines);

    void addLine(DataItem * item, int displayMode, int red, int green, int blue);

    // X and Y are
    void setPosition(float xScreenRatio, float yScreenRatio, float width, float height);

    void windowResized(float windowWidth, float windowHeight);

    void draw(sf::RenderWindow *window);
    void drawStat(sf::RenderWindow *window, DataItemConnector line);

    void getMinAndMaxValues();

    void mouseClicked(int x, int y);

    void setLegendsButtonColors();

    void mouseMoved(int x, int y);

    void drawHoveredInfo(sf::RenderWindow *window);

    void mouseHovering(int mouseX, int mouseY) override;

    void mouseClickedInside(int mouseX, int mouseY) override;

    void resized() override;

    void mouseEnter() override;

    void mouseLeave() override;
};


#endif //CREATURES_GRAPH_H
