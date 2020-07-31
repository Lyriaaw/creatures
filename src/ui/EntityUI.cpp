//
// Created by Amalric Lombard de Buffières on 7/31/20.
//

#include <iostream>
#include "EntityUI.h"
#include "colors/ColorUtils.h"

EntityUI::EntityUI(Entity *entity) : entity(entity) {
    this->vertexArray = sf::VertexArray(sf::Quads, 12);

    RGBColor color = hslToRgb(entity->getColor(), 1.f, 0.5f);
    this->color = sf::Color(color.getRed(), color.getGreen(), color.getBlue());
}


void EntityUI::draw(sf::RenderWindow *window, Camera *camera) {

    Point screenPoint = camera->getScreenCoordinates(this->entity->getPosition());

    float screenSize = this->entity->getSize() * camera->getZoom();

    // Colored body
    for (int it = 0; it < 4; it++) {
        double angle = ((2 * M_PI) * (it / 4.0)) - (0.25 * M_PI) + this->entity->getRotation();

        float currentX = screenPoint.getX() + (screenSize * cos(angle));
        float currentY = screenPoint.getY() + (screenSize * sin(angle));
        vertexArray[it] = sf::Vector2f(currentX, currentY);
        vertexArray[it].color = this->color;
    }



    // Eyes
    sf::Color eyeColor = sf::Color(0, 0, 0, 255);
    float eyeDistance = screenSize - (0.9f * camera->getZoom());

    int index = 1;
    for (int eyeRotationMultiplier = -1; eyeRotationMultiplier <= 1; eyeRotationMultiplier+= 2) {
        float eyeRotation = (float(eyeRotationMultiplier) * 0.4f) + this->entity->getRotation();

        float eyeX = (cos(eyeRotation) * eyeDistance) + screenPoint.getX();
        float eyeY = (sin(eyeRotation) * eyeDistance) + screenPoint.getY();


        for (int it = 0; it < 4; it++) {
            double angle = ((2 * M_PI) * (it / 4.0)) - (0.25 * M_PI) + this->entity->getRotation();

            float currentX = eyeX + ((2 * camera->getZoom()) * cos(angle));
            float currentY = eyeY + ((2 * camera->getZoom()) * sin(angle));

            int currentIt = it + (index * 4);
            vertexArray[currentIt] = sf::Vector2f(currentX, currentY);
            vertexArray[currentIt].color = eyeColor;
        }


        index++;


    }




    window->draw(vertexArray);

}