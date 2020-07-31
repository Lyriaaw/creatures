//
// Created by Amalric Lombard de Buffières on 7/31/20.
//

#ifndef CREATURES_UIENTITY_H
#define CREATURES_UIENTITY_H


#include <SFML/Graphics.hpp>
#include "ui/Camera.h"

class UiEntity {
private:
    sf::VertexArray body;
    sf::Color color;

public:
    UiEntity();

    void initialize(float color);
    void draw(sf::RenderWindow *window, Camera *camera, Point position, float size, float rotation, float hue);

};


#endif //CREATURES_UIENTITY_H
