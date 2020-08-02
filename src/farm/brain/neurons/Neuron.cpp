//
// Created by Amalric Lombard de Buffières on 8/2/20.
//

#include "Neuron.h"

float Neuron::getValue() const {
    return value;
}

float Neuron::getX() const {
    return x;
}

void Neuron::setX(float x) {
    Neuron::x = x;
}

float Neuron::getY() const {
    return y;
}

void Neuron::setY(float y) {
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
