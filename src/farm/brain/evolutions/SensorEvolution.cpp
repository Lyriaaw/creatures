//
// Created by Amalric Lombard de Buffières on 8/2/20.
//

#include <iostream>
#include "SensorEvolution.h"
#include "../neurons/SensorNeuron.h"

SensorEvolution::SensorEvolution(): Evolution() {

}

void SensorEvolution::generateFromRandom(BrainConnector * connector) {
    sensorLength = (((rand() % 300) / 100.f) + 2) * connector->getCreature()->getSize();
    sensorRotation = ((rand() % 200) / 100.f) - 1.f ;
    sensorColor = ((rand() % 999) / 1000.f);
}

void SensorEvolution::perform(BrainConnector * connector) {
    int index = connector->getCreature()->addVisionSensorSensor(sensorLength, sensorRotation, sensorColor);

    InputNeuron * distanceNeuron = new DistanceSensorNeuron(index);
    distanceNeuron->setHueOutline(sensorColor);
    distanceNeuron->setBrightnessOutline(0.4);
    distanceNeuron->setName("Distance");
    distanceNeuron->setGenerationNumber(this->generationNumber);
    connector->getBrain()->addInputNeuron(distanceNeuron);

    InputNeuron * brightnessNeuron = new BrightnessSensorNeuron(index);
    brightnessNeuron->setHueOutline(sensorColor);
    brightnessNeuron->setBrightnessOutline(0.4);
    brightnessNeuron->setName("Brightness");
    brightnessNeuron->setGenerationNumber(this->generationNumber + 1);
    connector->getBrain()->addInputNeuron(brightnessNeuron);

    InputNeuron * sizeNeuron = new SizeSensorNeuron(index);
    sizeNeuron->setHueOutline(sensorColor);
    sizeNeuron->setBrightnessOutline(0.4);
    sizeNeuron->setName("Size");
    sizeNeuron->setGenerationNumber(this->generationNumber + 2);
    connector->getBrain()->addInputNeuron(sizeNeuron);




}

std::string SensorEvolution::describe() {
    return std::to_string(this->generationNumber) + " => Sensor => length: " + std::to_string(sensorLength) + " rotation: " + std::to_string(sensorRotation) + " color: " + std::to_string(sensorColor);
}

Evolution * SensorEvolution::generateWithMate(Evolution * mate) {
    SensorEvolution * mateSensorEvolution(nullptr);

    if (mate == nullptr) {
//        std::cout << "NULL POINTER OF MATE SENSOR: " << generationNumber << std::endl;
    } else {
        mateSensorEvolution = dynamic_cast<SensorEvolution *>(mate);
        if (mateSensorEvolution == nullptr) {
            std::cout << "UNABLE TO CAST FATHER COLOR EVOLUTION" << std::endl;
        }
    }
    return generateFromCastedMate(mateSensorEvolution);
}

Evolution * SensorEvolution::generateFromCastedMate(SensorEvolution * mate) {
    SensorEvolution * childSensorEvolution = new SensorEvolution();
    childSensorEvolution->setGenerationNumber(getGenerationNumber());

    if (mate == nullptr) {
        childSensorEvolution->sensorLength = this->sensorLength;
        childSensorEvolution->sensorRotation = this->sensorRotation;
        childSensorEvolution->sensorColor = this->sensorColor;
        return childSensorEvolution;
    }


    // 3 chances out of 4 to simply select one of the parent's sensor
    if (rand() % 4 != 0) {
        if (rand() % 2 == 0) {
            childSensorEvolution->sensorLength = this->sensorLength;
            childSensorEvolution->sensorRotation = this->sensorRotation;
            childSensorEvolution->sensorColor = this->sensorColor;
        } else {
            childSensorEvolution->sensorLength = mate->sensorLength;
            childSensorEvolution->sensorRotation = mate->sensorRotation;
            childSensorEvolution->sensorColor = mate->sensorColor;
        }

        return childSensorEvolution;
    }



    float newLength = (this->sensorLength + mate->sensorLength) / 2.f;


    float newColor;
    float smallestHue = this->sensorColor;
    if (mate->sensorColor < smallestHue) {
        smallestHue = mate->sensorColor;
    }

    float rawDistance =  abs(this->sensorColor - mate->sensorColor);
    float hueDistances = std::min(rawDistance, 1.f - rawDistance);

    if (rawDistance < 1 - rawDistance) {
        newColor = smallestHue + (hueDistances / 2.f);
    } else {
        newColor = 1 + (smallestHue - (hueDistances / 2.f));
    }


    float newRotation;
    float smallestRotation = (this->sensorRotation + 1) / 2.f;
    if ((mate->sensorRotation + 1) / 2.f < smallestRotation) {
        smallestRotation = (mate->sensorRotation + 1) / 2.f;
    }

    float rawRotationDistance =  abs((this->sensorRotation + 1) / 2.f - (mate->sensorRotation + 1) / 2.f);
    float rotationDistances = std::min(rawRotationDistance, 1.f - rawRotationDistance);

    if (rawRotationDistance < 1 - rawRotationDistance) {
        newRotation = smallestRotation + (rotationDistances / 2.f);
    } else {
        newRotation = 1 + (smallestRotation - (rotationDistances / 2.f));
    }

    newRotation = (newRotation * 2.f) - 1.f;

    childSensorEvolution->sensorLength = newLength;
    childSensorEvolution->sensorRotation = newRotation;
    childSensorEvolution->sensorColor = newColor;

    return childSensorEvolution;
}