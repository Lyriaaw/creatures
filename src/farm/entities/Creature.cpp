//
// Created by Amalric Lombard de Buffières on 8/1/20.
//

#include <iostream>
#include "Creature.h"


Creature::Creature(Point position): Entity(position) {
    this->brightness = 0.5f;
//    this->rotation = ((rand() % 200) / 100.f) - 1.f;
    this->rotation = 0.f;

    mouthValue = 0;
    genitalsValue = 0;
}

int Creature::addVisionSensorSensor(float length, float rotation, float color) {
    sensorLengths.push_back(length);
    sensorRotations.push_back(rotation);
    sensorColors.push_back(color);

    sensorDistances.push_back(0);
    sensorBrightness.push_back(0);

    return getSensorCount() - 1;
}

double Creature::move() {



}



void Creature::addRotation(float rotationToAdd) {
    this->rotation += rotationToAdd;
}


int Creature::getSensorCount() {
    return sensorRotations.size();
}

float Creature::getSensorRotation(int index) {
    return sensorRotations.at(index);
}
float Creature::getSensorLength(int index) {
    return sensorLengths.at(index);
}
float Creature::getSensorDistance(int index) {
    return sensorDistances.at(index);
}
float Creature::getSensorBrightness(int index) {
    return sensorBrightness.at(index);
}
float Creature::getSensorSizes(int index) {
    return sensorSizes.at(index);
}

float Creature::getSensorColor(int index) {
    return sensorColors.at(index);
}



const std::vector<Point> &Creature::getSelectedChunks() const {
    return selectedChunks;
}

float Creature::getMouthRotation() const {
    return mouthRotation;
}

void Creature::setMouthRotation(float mouthRotation) {
    Creature::mouthRotation = mouthRotation;
}

float Creature::getGenitalsRotation() const {
    return genitalsRotation;
}

void Creature::setGenitalsRotation(float genitalsRotation) {
    Creature::genitalsRotation = genitalsRotation;
}

float Creature::getMouthValue() const {
    return mouthValue;
}

void Creature::setMouthValue(float mouthValue) {
    Creature::mouthValue = mouthValue;
}

float Creature::getGenitalsValue() const {
    return genitalsValue;
}

void Creature::setGenitalsValue(float genitalsValue) {
    Creature::genitalsValue = genitalsValue;
}



