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

void DistanceBarSensorEvolution::setSensorLength(float sensorLength) {
    DistanceBarSensorEvolution::sensorLength = sensorLength;
}

void DistanceBarSensorEvolution::setSensorRotation(float sensorRotation) {
    DistanceBarSensorEvolution::sensorRotation = sensorRotation;
}



void DistanceBarSensorEvolution::saveInMongo(MongoClient *client, int farmId) {
    auto builder = bsoncxx::builder::stream::document{};

    builder
            << "farmId" << farmId
            << "evolution_number" << this->generationNumber
            << "type" << "sensor_bar/distance"
            << "variables"
            << bsoncxx::builder::stream::open_document
            << "length" << this->sensorLength
            << "rotation" << this->sensorRotation
            << bsoncxx::builder::stream::close_document;

    bsoncxx::document::value doc_value = builder << bsoncxx::builder::stream::finalize;


    try {
        client->saveEvolution(doc_value.view());
    } catch (const std::exception& e) {
        std::cout << "Error while saving mouth evolution: " << " in thread " << std::this_thread::get_id() << " -> " << e.what() <<  std::endl;
    }
}



bsoncxx::builder::stream::document DistanceBarSensorEvolution::generateMongoVariables() {
    auto builder = bsoncxx::builder::stream::document{};

    builder
            << "evolution_number" << this->generationNumber
            << "type" << "sensor_bar/distance"
            << "variables"
            << bsoncxx::builder::stream::open_document
            << "length" << this->sensorLength
            << "rotation" << this->sensorRotation
            << bsoncxx::builder::stream::close_document;

    return builder;
}
