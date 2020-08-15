//
// Created by Amalric Lombard de Buffières on 8/9/20.
//

#include "Sensor.h"

double Sensor::getValue() const {
    return value;
}

void Sensor::setValue(double value) {
    Sensor::value = value;
}

Sensor::Sensor(Entity * entity) : value(0.0), entity(entity) {}

void Sensor::clearSelectedTiles() {
    this->selectedTiles.clear();
}

const std::vector<Point> &Sensor::getSelectedTiles() const {
    return selectedTiles;
}

void Sensor::passValueToNeuron() {
    this->connectedNeuron->setValue(this->value);
}

void Sensor::setConnectedNeuron(InputNeuron *connectedNeuron) {
    Sensor::connectedNeuron = connectedNeuron;
}

void Sensor::setEntity(Entity *entity) {
    Sensor::entity = entity;
}
