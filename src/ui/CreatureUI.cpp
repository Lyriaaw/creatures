//
// Created by Amalric Lombard de Buffières on 8/1/20.
//

#include "CreatureUI.h"

CreatureUI::CreatureUI(Creature *entity) : EntityUI(entity, 12, sf::Quads), creature(entity)  {
    sensors = sf::VertexArray(sf::Lines, entity->getSensorCount() * 2);
}

void CreatureUI::draw(sf::RenderWindow *window, Camera *camera, Entity * selectedEntity) {

    Point screenPoint = camera->getScreenCoordinates(this->entity->getPosition());

    float screenSize = this->entity->getSize() * camera->getZoom();

    // Colored body
    for (int it = 0; it < 4; it++) {
        double angle = ((2 * M_PI) * (it / 4.0)) - (0.25 * M_PI) + this->entity->getRotation();

        float currentX = screenPoint.getX() + (screenSize * cos(angle));
        float currentY = screenPoint.getY() + (screenSize * sin(angle));
        vertexArray[it] = sf::Vector2f(currentX, currentY);

        if (selectedEntity == this->entity) {
            vertexArray[it].color = sf::Color(255, 255, 255, 255);
        } else {
            vertexArray[it].color = this->color;
        }
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




    // Sensors
    sf::Color sensorColor = sf::Color(0, 0, 0, 255);

    for (int sensorIndex = 0; sensorIndex < creature->getSensorCount(); sensorIndex++) {
        float sensorDistance = creature->getSensorLength(sensorIndex) * camera->getZoom();

        float sensorRotation = creature->getSensorRotation(sensorIndex) + this->entity->getRotation();

        float sensorX = (cos(sensorRotation) * sensorDistance) + screenPoint.getX();
        float sensorY = (sin(sensorRotation) * sensorDistance) + screenPoint.getY();


        float currentX = sensorX + ((2 * camera->getZoom()) * cos(sensorRotation));
        float currentY = sensorY + ((2 * camera->getZoom()) * sin(sensorRotation));

        sensors[2 * sensorIndex].position = sf::Vector2f(currentX, currentY);
        sensors[2 * sensorIndex].color = sensorColor;

        sensors[(2 * sensorIndex) + 1].position = sf::Vector2f(screenPoint.getX(), screenPoint.getY());
        sensors[(2 * sensorIndex) + 1].color = sensorColor;


        index++;
    }




    window->draw(vertexArray);
    window->draw(sensors);

}