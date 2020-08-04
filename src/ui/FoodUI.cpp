//
// Created by Amalric Lombard de Buffières on 8/1/20.
//

#include <iostream>
#include "FoodUI.h"


FoodUI::FoodUI(Food *entity): EntityUI(entity, 15, sf::TriangleFan) {
}

void FoodUI::draw(sf::RenderWindow *window, Camera *camera, Entity * selectedEntity) {
    if (!this->entity->isExists()) {
        return;
//        this->color = sf::Color(0, 0, 0, 255);
    }


    Point screenPoint = camera->getScreenCoordinates(this->entity->getPosition());

    float screenSize = this->entity->getSize() * camera->getZoom();

    // Colored body

    vertexArray[0] = sf::Vector2f(screenPoint.getX(), screenPoint.getY());
    vertexArray[0].color = this->color;


    bool isSelected = selectedEntity == this->entity;

    for (int it = 1; it <= 14; it++) {
        double angle = (it * (2 * M_PI ) / 13.0);

        float currentX = screenPoint.getX() + (screenSize * cos(angle));
        float currentY = screenPoint.getY() + (screenSize * sin(angle));
        vertexArray[it] = sf::Vector2f(currentX, currentY);

        if (isSelected) {
            vertexArray[it].color = sf::Color(255, 255, 255);
        } else {
            vertexArray[it].color = this->color;
        }

    }


    window->draw(vertexArray);
}