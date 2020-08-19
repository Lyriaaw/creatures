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
    muscles = sf::VertexArray(sf::Quads, life->getExternalMuscles().size() * 8);
    energyLabel.setFont(*font);

    roundBody = sf::CircleShape(0.0);

    for (int it = 0; it < life->getExternalMuscles().size() * 2; it++) {
        roundMuscles.emplace_back(sf::CircleShape(0.0));
    }


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

    if (life->getType() == "ANIMAL") {
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
    } else {
        roundBody.setRadius(screenSize);
        roundBody.setFillColor(color);
        roundBody.setPosition(screenPoint.getX() - (screenSize), screenPoint.getY() - (screenSize));
        window->draw(roundBody);
    }



    float distance = screenSize;

    for (int it = 0; it < life->getExternalMuscles().size(); it++) {
        Muscle * currentMuscle = this->life->getExternalMuscles().at(it);

        float rotation = currentMuscle->getRotation();

        float muscleRotation = (float(rotation) + this->life->getEntity()->getRotation()) * float(M_PI);
        sf::Color muscleBackColor = sf::Color(0, 0, 0, 255);
        sf::Color muscleColor = sf::Color(200, 200, 200, 255);

        float muscleX = (cos(muscleRotation) * distance) + screenPoint.getX();
        float muscleY = (sin(muscleRotation) * distance) + screenPoint.getY();


        if (currentMuscle->getName() == "MOUTH") {
            muscleColor = this->color;
        }

        float relativeMuscleSize = this->life->getEntity()->getSize() / 3.f;
        if (currentMuscle->getName() == "GENITALS") {
            relativeMuscleSize = this->life->getEntity()->getSize() / 4.f;
        }
        if (currentMuscle->getName() == "CAPTURE_GROUND") {
            relativeMuscleSize = this->life->getEntity()->getSize() / 10.f;

        }
        if (currentMuscle->getName() == "DUPLICATION") {
            relativeMuscleSize = this->life->getEntity()->getSize() / 10.f;
            muscleColor = this->color;

        }

        float muscleSize = (currentMuscle->getValue() * relativeMuscleSize) * camera->getZoom();

        relativeMuscleSize *= camera->getZoom();

        roundMuscles.at((it * 2) + 0).setPosition(muscleX - (relativeMuscleSize), muscleY - (relativeMuscleSize));
        roundMuscles.at((it * 2) + 0).setRadius(relativeMuscleSize);
        roundMuscles.at((it * 2) + 0).setFillColor(muscleBackColor);

        roundMuscles.at((it * 2) + 1).setPosition(muscleX - (muscleSize), muscleY - (muscleSize));
        roundMuscles.at((it * 2) + 1).setRadius(muscleSize);
        roundMuscles.at((it * 2) + 1).setFillColor(muscleColor);

        window->draw(roundMuscles.at((it * 2) + 0));
        window->draw(roundMuscles.at((it * 2) + 1));




//        for (int jt = 0; jt < 4; jt++) {
//            double angle = ((2 * M_PI) * (jt / 4.0)) - (0.25 * M_PI) + (this->life->getEntity()->getRotation() * float(M_PI));
//
//            float currentX = muscleX + (((relativeMuscleSize) * camera->getZoom()) * cos(angle));
//            float currentY = muscleY + (((relativeMuscleSize) * camera->getZoom()) * sin(angle));
//
//            int currentIt = (it * 8) + jt;
//            muscles[currentIt] = sf::Vector2f(currentX, currentY);
//            muscles[currentIt].color = muscleBackColor;
//        }
//
//        for (int jt = 0; jt < 4; jt++) {
//            double angle = ((2 * M_PI) * (jt / 4.0)) - (0.25 * M_PI) + (this->life->getEntity()->getRotation() * float(M_PI));
//
//            float currentX = muscleX + ((muscleSize * camera->getZoom()) * cos(angle));
//            float currentY = muscleY + ((muscleSize * camera->getZoom()) * sin(angle));
//
//            int currentIt = (it * 8) + 4 + jt;
//            muscles[currentIt] = sf::Vector2f(currentX, currentY);
//            muscles[currentIt].color = muscleColor;
//        }

    }

//    window->draw(muscles);




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
    if (selectedEntity != nullptr && selectedEntity->getId() == this->life->getEntity()->getId()) {
        int index = 0;

        float energyDistance = (this->life->getEntity()->getSize() + 10) * camera->getZoom();
        energyBar[index + 0].position = sf::Vector2f(screenPoint.getX() - energyDistance, screenPoint.getY() - energyDistance);
        energyBar[index + 0].color =  sf::Color(0, 0, 0, 255);

        energyBar[index + 1].position = sf::Vector2f(screenPoint.getX() + energyDistance, screenPoint.getY() - energyDistance);
        energyBar[index + 1].color =  sf::Color(0, 0, 0, 255);

        energyBar[index + 2].position = sf::Vector2f(screenPoint.getX() + energyDistance, screenPoint.getY() - energyDistance + (5 * camera->getZoom()));
        energyBar[index + 2].color =  sf::Color(0, 0, 0, 255);

        energyBar[index + 3].position = sf::Vector2f(screenPoint.getX() - energyDistance, screenPoint.getY() - energyDistance + (5 * camera->getZoom()));
        energyBar[index + 3].color =  sf::Color(0, 0, 0, 255);

        index++;

        float energyRatio = (life->getEnergyManagement()->getEnergy() / life->getEntity()->getMass()) * energyDistance;

        energyBar[(index * 4) + 0].position = sf::Vector2f(screenPoint.getX() - energyRatio, screenPoint.getY() - energyDistance + 1);
        energyBar[(index * 4) + 0].color =  sf::Color(255, 255, 255, 255);

        energyBar[(index * 4) + 1].position = sf::Vector2f(screenPoint.getX() + energyRatio, screenPoint.getY() - energyDistance + 1);
        energyBar[(index * 4) + 1].color =  sf::Color(255, 255, 255, 255);

        energyBar[(index * 4) + 2].position = sf::Vector2f(screenPoint.getX() + energyRatio, screenPoint.getY() - energyDistance + (std::max(5.0 * camera->getZoom(), 1.0)) - 1);
        energyBar[(index * 4) + 2].color =  sf::Color(255, 255, 255, 255);

        energyBar[(index * 4) + 3].position = sf::Vector2f(screenPoint.getX() - energyRatio, screenPoint.getY() - energyDistance + (std::max(5.0 * camera->getZoom(), 1.0)) - 1);
        energyBar[(index * 4) + 3].color =  sf::Color(255, 255, 255, 255);

        window->draw(energyBar);



        energyLabel.setCharacterSize(3 * camera->getZoom());

        std::string energyText = std::to_string(life->getEnergyManagement()->getEnergy());
        energyLabel.setString(energyText);

        double xPosition = screenPoint.getX() - (energyLabel.getLocalBounds().width / 2);
        double yPosition = screenPoint.getY() - energyDistance - ((5 - energyLabel.getLocalBounds().height) / 2);

        energyLabel.setPosition(xPosition, yPosition);
        energyLabel.setFillColor(sf::Color(128, 128, 128, 255));
        window->draw(energyLabel);
    }
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
        BarSensor * currentSensor = (BarSensor *) this->life->getSensors().at(sensorIndex);
        if (currentSensor->getName() != "BRIGHTNESS_BAR" && currentSensor->getName() != "DISTANCE_BAR" && currentSensor->getName() != "SIZE_BAR") {
            continue;
        }


//        float currentSensorLength = this->life->getEntity()->getSize();
        float currentSensorLength = currentSensor->getLength();
//        if (selectedEntity == this->life->getEntity()) {
//            currentSensorLength = currentSensor->getLength();
//        }
        float currentSensorRotation = (currentSensor->getRotation() + this->life->getEntity()->getRotation()) * float(M_PI);


        sf::Color sensorColor = sf::Color(55, 55, 55, 255);

        if (currentSensor->getName() == "BRIGHTNESS_BAR") {
            BrightnessBarSensor * currentSensorBar = (BrightnessBarSensor *) this->life->getSensors().at(sensorIndex);
            RGBColor sensorRGB = RGBColor(currentSensorBar->getColor(), 1.f, 0.5f);
            sensorColor = sf::Color(sensorRGB.getRed(), sensorRGB.getGreen(), sensorRGB.getBlue(), 128);
        }

//

        if (currentSensor->getValue() > 0) {
//        if (false) {
            float currentSensorDistance = currentSensor->getLength() * (1.f - currentSensor->getValue());

            RGBColor sensorDistanceRGBColor = RGBColor(0.f, 1.f, 0.5f);
            sf::Color sensorDistanceColor = sf::Color(sensorDistanceRGBColor.getRed(), sensorDistanceRGBColor.getGreen(), sensorDistanceRGBColor.getBlue(), 255);


            sf::RectangleShape currentSensorValue(sf::Vector2f(currentSensorDistance * camera->getZoom(), 2 * camera->getZoom()));
            currentSensorValue.rotate(((currentSensor->getRotation() + this->life->getEntity()->getRotation())) * 180.f);
            currentSensorValue.setPosition(screenPoint.getX(), screenPoint.getY());
            currentSensorValue.setFillColor(sensorDistanceColor);
            window->draw(currentSensorValue);
        }




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
