//
// Created by Amalric Lombard de Buffières on 8/2/20.
//

#ifndef CREATURES_BRAINUI_H
#define CREATURES_BRAINUI_H


#include <SFML/Graphics.hpp>
#include "../../farm/brain/Brain.h"
#include "../dashboards/Graph.h"

class BrainUI {
private:

    Brain * brain;
    sf::Font * font;

    sf::RectangleShape background;
    std::vector<sf::CircleShape> neurons;
    std::vector<sf::Text> neuronNames;
    sf::VertexArray links;

    float x, y, width, height, windowWidth, windowHeight;

    Neuron * selectedNeuron;
    Graph * graph;
    DataItem * movement;
    DataItem * rotation;
    DataItem * mouth;
    DataItem * genitals;
    DataItem * grad0;
    DataItem * grad1;
    DataItem * gradm1;

public:
    BrainUI(Brain *brain, float x, float y, float width, float height, sf::Font * font, float windowWidth, float windowHeight);

    void initialize();
    void draw(sf::RenderWindow *window);

    bool mouseClicked(int x, int y);

    void updateAndDrawSelectedNeuron(sf::RenderWindow *window);
};


#endif //CREATURES_BRAINUI_H
