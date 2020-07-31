//
// Created by Amalric Lombard de Buffières on 7/31/20.
//

#include <iostream>
#include "EntityUI.h"
#include "../colors/RGBColor.h"
#include "../Utils.h"

EntityUI::EntityUI(Entity *entity) : entity(entity) {
    this->vertexArray = sf::VertexArray(sf::Quads, 4);

    RGBColor color = hslToRgb(entity->getColor(), 1.f, 0.5f);
    this->color = sf::Color(color.getRed(), color.getGreen(), color.getBlue());
}


void EntityUI::draw(sf::RenderWindow *window, Camera *camera) {

    Point screenPoint = camera->getScreenCoordinates(this->entity->getPosition());

    float screenSize = this->entity->getSize() * camera->getZoom();

    for (int it = 0; it < 4; it++) {
        double angle = ((2 * M_PI) * (it / 4.0)) - (0.25 * M_PI) + this->entity->getRotation();

        float currentX = screenPoint.getX() + (screenSize * cos(angle));
        float currentY = screenPoint.getY() + (screenSize * sin(angle));
        vertexArray[it] = sf::Vector2f(currentX, currentY);
        vertexArray[it].color = this->color;
    }

    window->draw(vertexArray);

}