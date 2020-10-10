
//
// Created by Amalric Lombard de Buffières on 8/10/20.
//

#ifndef CREATURES_LIFEUI_H
#define CREATURES_LIFEUI_H


#include <SFML/Graphics.hpp>
#include "../../farm/life/Life.h"

class LifeUI {
private:
    sf::Font * font;

    sf::Text energyLabel;
    sf::VertexArray barSensors;
    sf::VertexArray energyBar;

    std::vector<sf::CircleShape> roundMuscles;

    sf::Color color;
    sf::VertexArray body;

    Life * life;

public:
    LifeUI(Life *life, sf::Font * font);

    Life *getLife() const;

    void draw(sf::RenderWindow *window, Camera *camera, Entity * selectedEntity);

};


#endif //CREATURES_LIFEUI_H
