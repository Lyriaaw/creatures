//
// Created by Amalric Lombard de Buffières on 8/1/20.
//

#include <iostream>
#include "CreatureUI.h"
#include "colors/RGBColor.h"

CreatureUI::CreatureUI(Creature *entity) : EntityUI(entity, 12, sf::Quads), creature(entity)  {
    sensors = sf::VertexArray(sf::Lines, entity->getSensorCount() * 4);
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
            vertexArray[it].color =  this->color;
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

    window->draw(vertexArray);



    if (selectedEntity != this->creature) {
        return;
    }

    // Sensors

    for (int sensorIndex = 0; sensorIndex < creature->getSensorCount(); sensorIndex++) {
        RGBColor sensorRGB = RGBColor(this->creature->getSensorColor(sensorIndex), 1.f, 0.5f);
        sf::Color sensorColor = sf::Color(sensorRGB.getRed(), sensorRGB.getGreen(), sensorRGB.getBlue(), 255);

        float currentSensorLength = creature->getSensorLength(sensorIndex);
        float currentSensorRotation = creature->getSensorRotation(sensorIndex) + creature->getRotation();

        float sensorX = (cos(currentSensorRotation) * currentSensorLength) + creature->getPosition().getX();
        float sensorY = (sin(currentSensorRotation) * currentSensorLength) + creature->getPosition().getY();

        Point sensorScreenPlace = camera->getScreenCoordinates(Point(sensorX, sensorY));
        float currentX = sensorScreenPlace.getX();
        float currentY = sensorScreenPlace.getY();

        sensors[4 * sensorIndex].position = sf::Vector2f(currentX, currentY);
        sensors[4 * sensorIndex].color = sensorColor;

        sensors[(4 * sensorIndex) + 1].position = sf::Vector2f(screenPoint.getX(), screenPoint.getY());
        sensors[(4 * sensorIndex) + 1].color = sensorColor;




        sf::Color sensorDistanceColor = sf::Color(0, 0, 0, 255);
        float currentSensorDistance = creature->getSensorLength(sensorIndex) * (1.f - creature->getSensorDistance(sensorIndex));

        if (creature->getSensorDistance(sensorIndex) > 0) {
            std::cout << "Brightness: " << this->creature->getSensorBrightness(sensorIndex) << std::endl;
            RGBColor sensorDistanceRGBColor = RGBColor(this->creature->getSensorColor(sensorIndex), 1.f, this->creature->getSensorBrightness(sensorIndex));
            sensorDistanceColor = sf::Color(sensorDistanceRGBColor.getRed(), sensorDistanceRGBColor.getGreen(), sensorDistanceRGBColor.getBlue(), 255);
        } else {
            currentSensorDistance = 0;
        }
        float sensorDistanceX = (cos(currentSensorRotation) * currentSensorDistance) + creature->getPosition().getX();
        float sensorDistanceY = (sin(currentSensorRotation) * currentSensorDistance) + creature->getPosition().getY();

        Point sensorDistanceScreenPlace = camera->getScreenCoordinates(Point(sensorDistanceX, sensorDistanceY));
        float currentDistanceX = sensorDistanceScreenPlace.getX();
        float currentDistanceY = sensorDistanceScreenPlace.getY();

        sensors[4 * sensorIndex + 2].position = sf::Vector2f(currentDistanceX, currentDistanceY);
        sensors[4 * sensorIndex + 2].color = sensorDistanceColor;

        sensors[(4 * sensorIndex) + 3].position = sf::Vector2f(screenPoint.getX(), screenPoint.getY());
        sensors[(4 * sensorIndex) + 3].color = sensorDistanceColor;

        index++;
    }
    window->draw(sensors);






}