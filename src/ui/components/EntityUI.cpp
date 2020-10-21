//
// Created by Amalric Lombard de Buffières on 7/31/20.
//

#include "EntityUI.h"
#include "../colors/ColorUtils.h"
#include "../colors/RGBColor.h"
#include "../../farm/life/Life.h"


Entity *EntityUI::getEntity() const {
    return entity;
}

EntityUI::EntityUI(Entity *entity, sf::Font *font): entity(entity), font(font) {
    vertexArray = sf::VertexArray(sf::TriangleFan, 15);

    RGBColor rgbColor = RGBColor(entity->getColor(), 1.f, entity->getBrightness());
    this->color = sf::Color(rgbColor.getRed(), rgbColor.getGreen(), rgbColor.getBlue());
}

void EntityUI::draw(sf::RenderWindow *window, Camera *camera, Life *selectedLife) {
    Point screenPoint = camera->getScreenCoordinates(this->entity->getPosition());

    if (!camera->shouldDisplayPoint(screenPoint)) {
        return;
    }

    float screenSize = (this->entity->getSize()) * camera->getZoom();

    if (screenSize < 1) {
        return;
    }

    // Colored body

    vertexArray[0] = sf::Vector2f(screenPoint.getX(), screenPoint.getY());
    vertexArray[0].color = this->color;

    bool isSelected = false;
    if (selectedLife != nullptr) {
        for (auto const& currentAccessibleEntity : selectedLife->getCurrentAccessibleEntities()) {
            if (currentAccessibleEntity->getId() == entity->getId()) {
                isSelected = true;
            }
        }
    }



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

