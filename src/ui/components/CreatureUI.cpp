//
// Created by Amalric Lombard de Buffières on 8/1/20.
//

#include <iostream>
#include "CreatureUI.h"
#include "../colors/RGBColor.h"

CreatureUI::CreatureUI(Creature *entity, sf::Font *font) : EntityUI(entity, 20, sf::Quads), creature(entity)  {
    sensors = sf::VertexArray(sf::Lines, entity->getSensorCount() * 2);
    energy = sf::VertexArray(sf::Quads, 8);
    energyLabel.setFont(*font);
}

void CreatureUI::draw(sf::RenderWindow *window, Camera *camera, Entity * selectedEntity) {
    if (this->creature == nullptr) {
        std::cout << "NULL" << std::endl;
        return;
    }

    Point screenPoint = camera->getScreenCoordinates(this->entity->getPosition());

    if (!camera->shouldDisplayPoint(screenPoint)) {
        return;
    }

    float screenSize = this->entity->getSize() * camera->getZoom();

    // Colored body
    for (int it = 0; it < 4; it++) {
        double angle = ((2 * M_PI) * (it / 4.0)) - (0.25 * M_PI) + (this->entity->getRotation() * M_PI);

        float currentX = screenPoint.getX() + (screenSize * cos(angle));
        float currentY = screenPoint.getY() + (screenSize * sin(angle));
        vertexArray[it] = sf::Vector2f(currentX, currentY);
        vertexArray[it].color =  this->color;

//        if (selectedEntity == this->entity) {
//            vertexArray[it].color = sf::Color(255, 255, 255, 255);
//        } else {
//            vertexArray[it].color =  this->color;
//        }
    }


    float distance = screenSize;

    // Mouth
    float rotation = this->creature->getMouthRotation();

    float mouthRotation = (float(rotation) + this->entity->getRotation()) * float(M_PI);
    sf::Color mouthBackColor = sf::Color(0, 0, 0, 255);
    sf::Color mouthColor = sf::Color(200, 200, 200, 255);

    float mouthX = (cos(mouthRotation) * distance) + screenPoint.getX();
    float mouthY = (sin(mouthRotation) * distance) + screenPoint.getY();

    float mouthSize = this->creature->getMouthValue() * (this->entity->getSize() / 3.f);

    int index = 1;
    for (int it = 0; it < 4; it++) {
        double angle = ((2 * M_PI) * (it / 4.0)) - (0.25 * M_PI) + (this->entity->getRotation() * float(M_PI));

        float currentX = mouthX + (((this->entity->getSize() / 3.f) * camera->getZoom()) * cos(angle));
        float currentY = mouthY + (((this->entity->getSize() / 3.f) * camera->getZoom()) * sin(angle));

        int currentIt = it + (index * 4);
        vertexArray[currentIt] = sf::Vector2f(currentX, currentY);
        vertexArray[currentIt].color = mouthBackColor;
    }
    index++;

    for (int it = 0; it < 4; it++) {
        double angle = ((2 * M_PI) * (it / 4.0)) - (0.25 * M_PI) + (this->entity->getRotation() * float(M_PI));

        float currentX = mouthX + ((mouthSize * camera->getZoom()) * cos(angle));
        float currentY = mouthY + ((mouthSize * camera->getZoom()) * sin(angle));

        int currentIt = it + (index * 4);
        vertexArray[currentIt] = sf::Vector2f(currentX, currentY);
        vertexArray[currentIt].color = mouthColor;
    }
    index++;


    // Genitals
    float genitalsRotation = (float(this->creature->getGenitalsRotation()) + this->entity->getRotation()) * float(M_PI);

    float genitalsX = (cos(genitalsRotation) * distance) + screenPoint.getX();
    float genitalsY = (sin(genitalsRotation) * distance) + screenPoint.getY();

    float genitalsSize = this->creature->getGenitalsValue() * (this->entity->getSize() / 4.f);

    for (int it = 0; it < 4; it++) {
        double angle = ((2 * M_PI) * (it / 4.0)) - (0.25 * M_PI) + (this->entity->getRotation() * float(M_PI));

        float currentX = genitalsX + (((this->creature->getSize() / 4.f) * camera->getZoom()) * cos(angle));
        float currentY = genitalsY + (((this->creature->getSize() / 4.f) * camera->getZoom()) * sin(angle));

        int currentIt = it + (index * 4);
        vertexArray[currentIt] = sf::Vector2f(currentX, currentY);
        vertexArray[currentIt].color = mouthBackColor;
    }
    index++;

//    for (int it = 0; it < 4; it++) {
//        double angle = ((2 * M_PI) * (it / 4.0)) - (0.25 * M_PI) + (this->entity->getRotation() * float(M_PI));
//
//        float currentX = genitalsX + ((genitalsSize * camera->getZoom()) * cos(angle));
//        float currentY = genitalsY + ((genitalsSize * camera->getZoom()) * sin(angle));
//
//        int currentIt = it + (index * 4);
//        vertexArray[currentIt] = sf::Vector2f(currentX, currentY);
//        vertexArray[currentIt].color = this->color;
//    }
//    index++;
//
//
    window->draw(vertexArray);


    if (selectedEntity == this->entity) {
        index = 0;

        float energyDistance = (this->creature->getSize() + 10) * camera->getZoom();
        energy[index + 0].position = sf::Vector2f(screenPoint.getX() - energyDistance, screenPoint.getY() - energyDistance);
        energy[index + 0].color =  sf::Color(0, 0, 0, 255);

        energy[index + 1].position = sf::Vector2f(screenPoint.getX() + energyDistance, screenPoint.getY() - energyDistance);
        energy[index + 1].color =  sf::Color(0, 0, 0, 255);

        energy[index + 2].position = sf::Vector2f(screenPoint.getX() + energyDistance, screenPoint.getY() - energyDistance + (5 * camera->getZoom()));
        energy[index + 2].color =  sf::Color(0, 0, 0, 255);

        energy[index + 3].position = sf::Vector2f(screenPoint.getX() - energyDistance, screenPoint.getY() - energyDistance + (5 * camera->getZoom()));
        energy[index + 3].color =  sf::Color(0, 0, 0, 255);

        index++;

        float energyRatio = (this->getEntity()->getEnergy() / this->getEntity()->getMaxEnergy()) * energyDistance;

        energy[(index * 4) + 0].position = sf::Vector2f(screenPoint.getX() - energyRatio, screenPoint.getY() - energyDistance + 1);
        energy[(index * 4) + 0].color =  sf::Color(255, 255, 255, 255);

        energy[(index * 4) + 1].position = sf::Vector2f(screenPoint.getX() + energyRatio, screenPoint.getY() - energyDistance + 1);
        energy[(index * 4) + 1].color =  sf::Color(255, 255, 255, 255);

        energy[(index * 4) + 2].position = sf::Vector2f(screenPoint.getX() + energyRatio, screenPoint.getY() - energyDistance + (5 * camera->getZoom()) - 1);
        energy[(index * 4) + 2].color =  sf::Color(255, 255, 255, 255);

        energy[(index * 4) + 3].position = sf::Vector2f(screenPoint.getX() - energyRatio, screenPoint.getY() - energyDistance + (5 * camera->getZoom()) - 1);
        energy[(index * 4) + 3].color =  sf::Color(255, 255, 255, 255);

        window->draw(energy);



        energyLabel.setCharacterSize(3 * camera->getZoom());

        std::string energyText = std::to_string(creature->getEnergy());
        energyLabel.setString(energyText);

        double xPosition = screenPoint.getX() - (energyLabel.getLocalBounds().width / 2);
        double yPosition = screenPoint.getY() - energyDistance - ((5 - energyLabel.getLocalBounds().height) / 2);

        energyLabel.setPosition(xPosition, yPosition);
        energyLabel.setFillColor(sf::Color(128, 128, 128, 255));
        window->draw(energyLabel);
    }









    // Sensors
    for (int sensorIndex = 0; sensorIndex < creature->getSensorCount(); sensorIndex++) {

//        if (creature->getSensorDistance(sensorIndex) > 0) {
        if (false) {
            float currentSensorDistance = creature->getSensorLength(sensorIndex) * (1.f - creature->getSensorDistance(sensorIndex));

            RGBColor sensorDistanceRGBColor = RGBColor(this->creature->getSensorColor(sensorIndex), 0.f, this->creature->getSensorBrightness(sensorIndex));
            sf::Color sensorDistanceColor = sf::Color(sensorDistanceRGBColor.getRed(), sensorDistanceRGBColor.getGreen(), sensorDistanceRGBColor.getBlue(), 255);


            sf::RectangleShape currentSensorValue(sf::Vector2f(currentSensorDistance * camera->getZoom(), 2 * camera->getZoom()));
            currentSensorValue.rotate((creature->getRotation() + creature->getSensorRotation(sensorIndex)) * 180.f);
            currentSensorValue.setPosition(screenPoint.getX(), screenPoint.getY());
            currentSensorValue.setFillColor(sensorDistanceColor);
            window->draw(currentSensorValue);
        }


        RGBColor sensorRGB = RGBColor(this->creature->getSensorColor(sensorIndex), 1.f, 0.5f);
        sf::Color sensorColor = sf::Color(sensorRGB.getRed(), sensorRGB.getGreen(), sensorRGB.getBlue(), 128);

        float currentSensorLength = creature->getSize();
        if (selectedEntity == this->creature) {
            currentSensorLength = creature->getSensorLength(sensorIndex);
        }
        float currentSensorRotation = (creature->getSensorRotation(sensorIndex) + this->entity->getRotation()) * float(M_PI);

        float sensorX = (cos(currentSensorRotation) * currentSensorLength) + creature->getPosition().getX();
        float sensorY = (sin(currentSensorRotation) * currentSensorLength) + creature->getPosition().getY();

        Point sensorScreenPlace = camera->getScreenCoordinates(Point(sensorX, sensorY));
        float currentX = sensorScreenPlace.getX();
        float currentY = sensorScreenPlace.getY();

        sensors[2 * sensorIndex].position = sf::Vector2f(currentX, currentY);
        sensors[2 * sensorIndex].color = sensorColor;

        sensors[(2 * sensorIndex) + 1].position = sf::Vector2f(screenPoint.getX(), screenPoint.getY());
        sensors[(2 * sensorIndex) + 1].color = sensorColor;
    }
    window->draw(sensors);






}