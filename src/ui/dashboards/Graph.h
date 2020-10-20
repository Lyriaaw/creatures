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
#include "../elements/UiButton.h"
#include "GraphControlCenter.h"

class Graph: public UiComponent {
protected:
    std::string name;
    std::vector<DataItemConnector> lines;

    std::vector<UiComponent *> uiComponents;

    std::vector<UiBackground *> labelsBackgrounds;
    std::vector<UiLabel *> labels;

    double min, max;

    double tickWidthRatio, tickHeightRatio;

    double lastMouseX;
    int hoveredTick;

    sf::RectangleShape background;




    std::vector<UiButton *> doNotShowButtons;
    std::vector<UiButton *> showNormalButtons;
    std::vector<UiButton *> showAveragedButtons;
    std::vector<UiLabel *> hoveredInfoValues;

    UiBackground * hoveredValuesBackground;
    UiBackground * hoveredTickMarker;

    GraphControlCenter * graphControlCenter;


public:
    Graph(const std::string &name, float xRatio, float yRatio, float widthRatio, float heightRatio);

    const std::string &getName() const;

    void setName(const std::string &name);

    const std::vector<DataItemConnector> &getLines() const;

    void setLines(const std::vector<DataItemConnector> &lines);

    void addLine(DataItem * item, int displayMode, int red, int green, int blue);


    void windowResized(float windowWidth, float windowHeight);

    void draw(sf::RenderWindow *window);
    void drawStat(sf::RenderWindow *window, DataItemConnector line);

    void getMinAndMaxValues();

    void mouseClicked(int x, int y);

    void setLegendsButtonColors();

    void mouseMoved(int mouseX, int mouseY, int previousX, int previousY);

    void mouseHovering(int mouseX, int mouseY) override;

    void mouseClickedInside(int mouseX, int mouseY) override;

    void resized() override;

    void mouseEnter() override;

    void mouseLeave() override;

    void updateHoveredTickInformation();

    void setGraphControlCenter(GraphControlCenter *graphControlCenter);
};


#endif //CREATURES_GRAPH_H
