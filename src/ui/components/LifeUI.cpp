//
// Created by Amalric Lombard de Buffières on 8/10/20.
//

#include "LifeUI.h"
#include "../colors/RGBColor.h"
#include "../../farm/life/sensors/bars/BarSensor.h"
#include "../../farm/life/sensors/bars/BrightnessBarSensor.h"

LifeUI::LifeUI(Life *life, sf::Font *font): life(life), font(font) {
    barSensors = sf::VertexArray(sf::Lines, life->getSensors().size() * 2);
    energyBar = sf::VertexArray(sf::Quads, 8);
    body = sf::VertexArray(sf::Quads, 4);
    energyLabel.setFont(*font);

    RGBColor rgbColor = RGBColor(life->getEntity()->getColor(), 1.f, life->getEntity()->getBrightness());
    this->color = sf::Color(rgbColor.getRed(), rgbColor.getGreen(), rgbColor.getBlue());
}

Life *LifeUI::getLife() const {
    return life;
}

void LifeUI::draw(sf::RenderWindow *window, Camera *camera, Entity *selectedEntity) {
    Point screenPoint = camera->getScreenCoordinates(this->life->getEntity()->getPosition());

    if (!camera->shouldDisplayPoint(screenPoint)) {
        return;
    }

    float screenSize = this->life->getEntity()->getSize() * camera->getZoom();

    // Colored body
    for (int it = 0; it < 4; it++) {
        double angle = ((2 * M_PI) * (it / 4.0)) - (0.25 * M_PI) + (this->life->getEntity()->getRotation() * M_PI);

        float currentX = screenPoint.getX() + (screenSize * cos(angle));
        float currentY = screenPoint.getY() + (screenSize * sin(angle));
        body[it] = sf::Vector2f(currentX, currentY);
        body[it].color =  this->color;

        if (selectedEntity == this->life->getEntity()) {
            body[it].color = sf::Color(255, 255, 255, 255);
        } else {
            body[it].color =  this->color;
        }
    }

    window->draw(body);


//    float distance = screenSize;
//
//    // Mouth
//    float rotation = this->entity->getMouthRotation();
//
//    float mouthRotation = (float(rotation) + this->life->getEntity()->getRotation()) * float(M_PI);
//    sf::Color mouthBackColor = sf::Color(0, 0, 0, 255);
//    sf::Color mouthColor = sf::Color(200, 200, 200, 255);
//
//    float mouthX = (cos(mouthRotation) * distance) + screenPoint.getX();
//    float mouthY = (sin(mouthRotation) * distance) + screenPoint.getY();
//
//    float mouthSize = this->entity->getMouthValue() * (this->life->getEntity()->getSize() / 3.f);
//
//    int index = 1;
//    for (int it = 0; it < 4; it++) {
//        double angle = ((2 * M_PI) * (it / 4.0)) - (0.25 * M_PI) + (this->life->getEntity()->getRotation() * float(M_PI));
//
//        float currentX = mouthX + (((this->life->getEntity()->getSize() / 3.f) * camera->getZoom()) * cos(angle));
//        float currentY = mouthY + (((this->life->getEntity()->getSize() / 3.f) * camera->getZoom()) * sin(angle));
//
//        int currentIt = it + (index * 4);
//        vertexArray[currentIt] = sf::Vector2f(currentX, currentY);
//        vertexArray[currentIt].color = mouthBackColor;
//    }
//    index++;
//
//    for (int it = 0; it < 4; it++) {
//        double angle = ((2 * M_PI) * (it / 4.0)) - (0.25 * M_PI) + (this->life->getEntity()->getRotation() * float(M_PI));
//
//        float currentX = mouthX + ((mouthSize * camera->getZoom()) * cos(angle));
//        float currentY = mouthY + ((mouthSize * camera->getZoom()) * sin(angle));
//
//        int currentIt = it + (index * 4);
//        vertexArray[currentIt] = sf::Vector2f(currentX, currentY);
//        vertexArray[currentIt].color = mouthColor;
//    }
//    index++;
//
//
//    // Genitals
//    float genitalsRotation = (float(this->entity->getGenitalsRotation()) + this->life->getEntity()->getRotation()) * float(M_PI);
//
//    float genitalsX = (cos(genitalsRotation) * distance) + screenPoint.getX();
//    float genitalsY = (sin(genitalsRotation) * distance) + screenPoint.getY();
//
//    float genitalsSize = this->entity->getGenitalsValue() * (this->life->getEntity()->getSize() / 4.f);
//
//    for (int it = 0; it < 4; it++) {
//        double angle = ((2 * M_PI) * (it / 4.0)) - (0.25 * M_PI) + (this->life->getEntity()->getRotation() * float(M_PI));
//
//        float currentX = genitalsX + (((this->entity->getSize() / 4.f) * camera->getZoom()) * cos(angle));
//        float currentY = genitalsY + (((this->entity->getSize() / 4.f) * camera->getZoom()) * sin(angle));
//
//        int currentIt = it + (index * 4);
//        vertexArray[currentIt] = sf::Vector2f(currentX, currentY);
//        vertexArray[currentIt].color = mouthBackColor;
//    }
//    index++;
//
////    for (int it = 0; it < 4; it++) {
////        double angle = ((2 * M_PI) * (it / 4.0)) - (0.25 * M_PI) + (this->life->getEntity()->getRotation() * float(M_PI));
////
////        float currentX = genitalsX + ((genitalsSize * camera->getZoom()) * cos(angle));
////        float currentY = genitalsY + ((genitalsSize * camera->getZoom()) * sin(angle));
////
////        int currentIt = it + (index * 4);
////        vertexArray[currentIt] = sf::Vector2f(currentX, currentY);
////        vertexArray[currentIt].color = this->color;
////    }
////    index++;
////
////
//    window->draw(vertexArray);
//
//
//    if (selectedEntity == this->life->getEntity()) {
//        index = 0;
//
//        float energyDistance = (this->entity->getSize() + 10) * camera->getZoom();
//        energy[index + 0].position = sf::Vector2f(screenPoint.getX() - energyDistance, screenPoint.getY() - energyDistance);
//        energy[index + 0].color =  sf::Color(0, 0, 0, 255);
//
//        energy[index + 1].position = sf::Vector2f(screenPoint.getX() + energyDistance, screenPoint.getY() - energyDistance);
//        energy[index + 1].color =  sf::Color(0, 0, 0, 255);
//
//        energy[index + 2].position = sf::Vector2f(screenPoint.getX() + energyDistance, screenPoint.getY() - energyDistance + (5 * camera->getZoom()));
//        energy[index + 2].color =  sf::Color(0, 0, 0, 255);
//
//        energy[index + 3].position = sf::Vector2f(screenPoint.getX() - energyDistance, screenPoint.getY() - energyDistance + (5 * camera->getZoom()));
//        energy[index + 3].color =  sf::Color(0, 0, 0, 255);
//
//        index++;
//
//        float energyRatio = (this->getEntity()->getEnergy() / this->getEntity()->getMaxEnergy()) * energyDistance;
//
//        energy[(index * 4) + 0].position = sf::Vector2f(screenPoint.getX() - energyRatio, screenPoint.getY() - energyDistance + 1);
//        energy[(index * 4) + 0].color =  sf::Color(255, 255, 255, 255);
//
//        energy[(index * 4) + 1].position = sf::Vector2f(screenPoint.getX() + energyRatio, screenPoint.getY() - energyDistance + 1);
//        energy[(index * 4) + 1].color =  sf::Color(255, 255, 255, 255);
//
//        energy[(index * 4) + 2].position = sf::Vector2f(screenPoint.getX() + energyRatio, screenPoint.getY() - energyDistance + (5 * camera->getZoom()) - 1);
//        energy[(index * 4) + 2].color =  sf::Color(255, 255, 255, 255);
//
//        energy[(index * 4) + 3].position = sf::Vector2f(screenPoint.getX() - energyRatio, screenPoint.getY() - energyDistance + (5 * camera->getZoom()) - 1);
//        energy[(index * 4) + 3].color =  sf::Color(255, 255, 255, 255);
//
//        window->draw(energy);
//
//
//
//        energyLabel.setCharacterSize(3 * camera->getZoom());
//
//        std::string energyText = std::to_string(entity->getEnergy());
//        energyLabel.setString(energyText);
//
//        double xPosition = screenPoint.getX() - (energyLabel.getLocalBounds().width / 2);
//        double yPosition = screenPoint.getY() - energyDistance - ((5 - energyLabel.getLocalBounds().height) / 2);
//
//        energyLabel.setPosition(xPosition, yPosition);
//        energyLabel.setFillColor(sf::Color(128, 128, 128, 255));
//        window->draw(energyLabel);
//    }
//
//
//
//
//
//
//
//
//
    // Sensors
    for (int sensorIndex = 0; sensorIndex < this->life->getSensors().size(); sensorIndex++) {

//        if (entity->getSensorDistance(sensorIndex) > 0) {
//        if (false) {
//            float currentSensorDistance = entity->getSensorLength(sensorIndex) * (1.f - entity->getSensorDistance(sensorIndex));
//
//            RGBColor sensorDistanceRGBColor = RGBColor(this->entity->getSensorColor(sensorIndex), 0.f, this->entity->getSensorBrightness(sensorIndex));
//            sf::Color sensorDistanceColor = sf::Color(sensorDistanceRGBColor.getRed(), sensorDistanceRGBColor.getGreen(), sensorDistanceRGBColor.getBlue(), 255);
//
//
//            sf::RectangleShape currentSensorValue(sf::Vector2f(currentSensorDistance * camera->getZoom(), 2 * camera->getZoom()));
//            currentSensorValue.rotate((entity->getRotation() + entity->getSensorRotation(sensorIndex)) * 180.f);
//            currentSensorValue.setPosition(screenPoint.getX(), screenPoint.getY());
//            currentSensorValue.setFillColor(sensorDistanceColor);
//            window->draw(currentSensorValue);
//        }

        BrightnessBarSensor * currentSensor = (BrightnessBarSensor *) this->life->getSensors().at(sensorIndex);
        if (currentSensor->getName() != "BRIGHTNESS_BAR") {
            continue;
        }

        RGBColor sensorRGB = RGBColor(currentSensor->getColor(), 1.f, 0.5f);
        sf::Color sensorColor = sf::Color(sensorRGB.getRed(), sensorRGB.getGreen(), sensorRGB.getBlue(), 128);

//        float currentSensorLength = this->life->getEntity()->getSize();
        float currentSensorLength = currentSensor->getLength();
//        if (selectedEntity == this->life->getEntity()) {
//            currentSensorLength = currentSensor->getLength();
//        }
        float currentSensorRotation = (currentSensor->getRotation() + this->life->getEntity()->getRotation()) * float(M_PI);
//
        float sensorX = (cos(currentSensorRotation) * currentSensorLength) + life->getEntity()->getPosition().getX();
        float sensorY = (sin(currentSensorRotation) * currentSensorLength) + life->getEntity()->getPosition().getY();

        Point sensorScreenPlace = camera->getScreenCoordinates(Point(sensorX, sensorY));
        float currentX = sensorScreenPlace.getX();
        float currentY = sensorScreenPlace.getY();
//        std::cout << "X: " << currentX << "(" << sensorX << ") Y: " << currentY << "(" << sensorY << ") Rotation: " << currentSensorRotation << " Length: " << currentSensorLength  << std::endl;
//
        barSensors[2 * sensorIndex].position = sf::Vector2f(currentX, currentY);
        barSensors[2 * sensorIndex].color = sensorColor;

        barSensors[(2 * sensorIndex) + 1].position = sf::Vector2f(screenPoint.getX(), screenPoint.getY());
        barSensors[(2 * sensorIndex) + 1].color = sensorColor;
    }
    window->draw(barSensors);



}
