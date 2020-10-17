//
// Created by Amalric Lombard de Buffières on 10/14/20.
//

#include "DistanceBarSensorEvolution.h"


#include <iostream>
#include "../../brain/neurons/SensorNeuron.h"
#include "../../life/sensors/bars/BarSensor.h"
#include "../../life/sensors/bars/DistanceBarSensor.h"
#include "../../life/sensors/bars/BrightnessBarSensor.h"

DistanceBarSensorEvolution::DistanceBarSensorEvolution(): Evolution() {

}

void DistanceBarSensorEvolution::generateFromRandom(Life * life) {
    sensorLength = (((rand() % 300) / 100.f) + 2);
    sensorRotation = ((rand() % 200) / 100.f) - 1.f ;
}

void DistanceBarSensorEvolution::perform(Life * life) {
    BarSensor * barSensor = new DistanceBarSensor(life->getEntity(), sensorRotation, sensorLength);


    InputNeuron * inputNeuron = new InputNeuron();
    inputNeuron->setHueOutline(0);
    inputNeuron->setBrightnessOutline(0);
    inputNeuron->setName("Distance");
    inputNeuron->setGenerationNumber(this->generationNumber);


    barSensor->setConnectedNeuron(inputNeuron);

    life->getBrain()->addInputNeuron(inputNeuron);

    life->addSensor(barSensor);

}

std::string DistanceBarSensorEvolution::describe() {
    return std::to_string(this->generationNumber) + " => Distance Sensor Bar => length: " + std::to_string(sensorLength) + " rotation: " + std::to_string(sensorRotation);
}

Evolution * DistanceBarSensorEvolution::generateWithMate(Evolution * mate) {
    DistanceBarSensorEvolution * mateSensorEvolution(nullptr);

    if (mate == nullptr) {
//        std::cout << "NULL POINTER OF MATE SENSOR: " << generationNumber << std::endl;
    } else {
        mateSensorEvolution = dynamic_cast<DistanceBarSensorEvolution *>(mate);
        if (mateSensorEvolution == nullptr) {
            std::cout << "UNABLE TO CAST FATHER COLOR EVOLUTION" << std::endl;
        }
    }
    return generateFromCastedMate(mateSensorEvolution);
}

Evolution * DistanceBarSensorEvolution::generateFromCastedMate(DistanceBarSensorEvolution * mate) {
    DistanceBarSensorEvolution * childSensorEvolution = new DistanceBarSensorEvolution();
    childSensorEvolution->setGenerationNumber(getGenerationNumber());

    if (mate == nullptr) {
        childSensorEvolution->sensorLength = this->sensorLength;
        childSensorEvolution->sensorRotation = this->sensorRotation;
        return childSensorEvolution;
    }


    // 3 chances out of 4 to simply select one of the parent's sensor
    if (rand() % MUTATION_RATIO != 0) {
        if (rand() % 2 == 0) {
            childSensorEvolution->sensorLength = this->sensorLength;
            childSensorEvolution->sensorRotation = this->sensorRotation;
        } else {
            childSensorEvolution->sensorLength = mate->sensorLength;
            childSensorEvolution->sensorRotation = mate->sensorRotation;
        }

        return childSensorEvolution;
    }

    childSensorEvolution->sensorLength = (((rand() % 300) / 100.f) + 2);
    childSensorEvolution->sensorRotation = ((rand() % 200) / 100.f) - 1.f ;

    return childSensorEvolution;
}


std::string DistanceBarSensorEvolution::getName() {
    return "Distance bar sensor";
}
