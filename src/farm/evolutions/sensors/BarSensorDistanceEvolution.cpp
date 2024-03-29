//
// Created by Amalric Lombard de Buffières on 8/2/20.
//

#include <iostream>
#include "BarSensorDistanceEvolution.h"
#include "../../life/sensors/bars/BarSensor.h"
#include "../../life/sensors/bars/DistanceBarSensor.h"
#include "../../life/sensors/bars/BrightnessBarSensor.h"

BarSensorDistanceEvolution::BarSensorDistanceEvolution(): Evolution() {

}

void BarSensorDistanceEvolution::generateFromRandom(Life * life) {
    sensorLength = (((rand() % 300) / 100.f) + 2);
    sensorRotation = ((rand() % 200) / 100.f) - 1.f ;
    sensorColor = ((rand() % 999) / 1000.f);
}

void BarSensorDistanceEvolution::perform(Life * life) {
    BarSensor * barSensor = new DistanceBarSensor(life->getEntity(), sensorRotation, sensorLength);


    InputNeuron * inputNeuron = new InputNeuron();
    inputNeuron->setHueOutline(sensorColor);
    inputNeuron->setBrightnessOutline(0.4);
    inputNeuron->setName("Brightness -" + std::to_string(sensorLength));
    inputNeuron->setGenerationNumber(this->generationNumber);


    barSensor->setConnectedNeuron(inputNeuron);

    life->getBrain()->addInputNeuron(inputNeuron);

    life->addSensor(barSensor);

}

std::string BarSensorDistanceEvolution::describe() {
    return std::to_string(this->generationNumber) + " => Sensor => length: " + std::to_string(sensorLength) + " rotation: " + std::to_string(sensorRotation) + " color: " + std::to_string(sensorColor);
}

Evolution * BarSensorDistanceEvolution::generateWithMate(Evolution * mate) {
    BarSensorDistanceEvolution * mateSensorEvolution(nullptr);

    if (mate == nullptr) {
//        std::cout << "NULL POINTER OF MATE SENSOR: " << generationNumber << std::endl;
    } else {
        mateSensorEvolution = dynamic_cast<BarSensorDistanceEvolution *>(mate);
        if (mateSensorEvolution == nullptr) {
            std::cout << "UNABLE TO CAST FATHER COLOR EVOLUTION" << std::endl;
        }
    }
    return generateFromCastedMate(mateSensorEvolution);
}

Evolution * BarSensorDistanceEvolution::generateFromCastedMate(BarSensorDistanceEvolution * mate) {
    BarSensorDistanceEvolution * childSensorEvolution = new BarSensorDistanceEvolution();
    childSensorEvolution->setGenerationNumber(getGenerationNumber());

    if (mate == nullptr) {
        childSensorEvolution->sensorLength = this->sensorLength;
        childSensorEvolution->sensorRotation = this->sensorRotation;
        childSensorEvolution->sensorColor = this->sensorColor;
        return childSensorEvolution;
    }


    // 3 chances out of 4 to simply select one of the parent's sensor
    if (rand() % MUTATION_RATIO != 0) {
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

    childSensorEvolution->sensorLength = sensorLength;
    childSensorEvolution->sensorRotation = ((rand() % 200) / 100.f) - 1.f ;
    childSensorEvolution->sensorColor = ((rand() % 999) / 1000.f);



//    float newLength = (this->sensorLength + mate->sensorLength) / 2.f;
//
//
//    float newColor;
//    float smallestHue = this->sensorColor;
//    if (mate->sensorColor < smallestHue) {
//        smallestHue = mate->sensorColor;
//    }
//
//    float rawDistance =  abs(this->sensorColor - mate->sensorColor);
//    float hueDistances = std::min(rawDistance, 1.f - rawDistance);
//
//    if (rawDistance < 1 - rawDistance) {
//        newColor = smallestHue + (hueDistances / 2.f);
//    } else {
//        newColor = 1 + (smallestHue - (hueDistances / 2.f));
//    }
//
//
//    float newRotation;
//    float smallestRotation = (this->sensorRotation + 1) / 2.f;
//    if ((mate->sensorRotation + 1) / 2.f < smallestRotation) {
//        smallestRotation = (mate->sensorRotation + 1) / 2.f;
//    }
//
//    float rawRotationDistance =  abs((this->sensorRotation + 1) / 2.f - (mate->sensorRotation + 1) / 2.f);
//    float rotationDistances = std::min(rawRotationDistance, 1.f - rawRotationDistance);
//
//    if (rawRotationDistance < 1 - rawRotationDistance) {
//        newRotation = smallestRotation + (rotationDistances / 2.f);
//    } else {
//        newRotation = 1 + (smallestRotation - (rotationDistances / 2.f));
//    }
//
//    newRotation = (newRotation * 2.f) - 1.f;
//
//    childSensorEvolution->sensorLength = newLength;
//    childSensorEvolution->sensorRotation = newRotation;
//    childSensorEvolution->sensorColor = newColor;

    return childSensorEvolution;
}