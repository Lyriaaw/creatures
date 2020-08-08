//
// Created by Amalric Lombard de Buffières on 8/8/20.
//

#ifndef CREATURES_GRAPH_H
#define CREATURES_GRAPH_H


#include <string>
#include <SFML/Graphics.hpp>
#include "DataItemConnector.h"

class Graph {
private:
    std::string name;
    std::vector<DataItemConnector> lines;

    float xScreenRatio, yScreenRatio;
    float widthScreenRatio, heightScreenRatio;

    float x, y, width, height;

    double min, max;

    double widthRatio, heightRatio;

    sf::RectangleShape background;

public:
    Graph(const std::string &name);

    const std::string &getName() const;

    void setName(const std::string &name);

    const std::vector<DataItemConnector> &getLines() const;

    void setLines(const std::vector<DataItemConnector> &lines);

    void addLine(DataItem * item, int red, int green, int blue);

    // X and Y are
    void setPosition(float xScreenRatio, float yScreenRatio, float width, float height);

    void windowResized(float windowWidth, float windowHeight);

    void draw(sf::RenderWindow *window);
    void drawStat(sf::RenderWindow *window, DataItemConnector line);

    void getMinAndMaxValues();


};


#endif //CREATURES_GRAPH_H
