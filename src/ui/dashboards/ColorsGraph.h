//
// Created by Amalric Lombard de Buffières on 10/14/20.
//

#ifndef CREATURES_COLORSGRAPH_H
#define CREATURES_COLORSGRAPH_H


#include <string>
#include "../../farm/statistics/GeneGraphAnalyser.h"
#include "Graph.h"

class ColorsGraph: public Graph {
private:
    GeneGraphAnalyser * dataAnalyser;

    void getMinAndMaxValues();

    sf::Texture texture;

    sf::Sprite sprite;

public:
    ColorsGraph(const std::string &name, sf::Font *font, GeneGraphAnalyser *dataAnalyser);

    void draw(sf::RenderWindow *window);


    void windowResized(float windowWidth, float windowHeight);
};


#endif //CREATURES_COLORSGRAPH_H
