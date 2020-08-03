//
// Created by Amalric Lombard de Buffières on 8/2/20.
//

#include <cmath>
#include "Neuron.h"



int Neuron::getX() const {
    return x;
}

void Neuron::setX(int x) {
    Neuron::x = x;
}

int Neuron::getY() const {
    return y;
}

void Neuron::setY(int y) {
    Neuron::y = y;
}

float Neuron::getHueOutline() const {
    return hueOutline;
}

void Neuron::setHueOutline(float hueOutline) {
    Neuron::hueOutline = hueOutline;
}

const std::string &Neuron::getName() const {
    return name;
}

void Neuron::setName(const std::string &name) {
    Neuron::name = name;
}

float Neuron::getBrightnessOutline() const {
    return brightnessOutline;
}

void Neuron::setBrightnessOutline(float brightnessOutline) {
    Neuron::brightnessOutline = brightnessOutline;
}

void Neuron::clear(){
    this->value = 0;
}


void Neuron::addValue(float addedValue) {
    this->value += addedValue;
}

int Neuron::getGenerationNumber() const {
    return generationNumber;
}

void Neuron::setGenerationNumber(int generationNumber) {
    Neuron::generationNumber = generationNumber;
}
