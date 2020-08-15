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

Sensor::Sensor(Body * entity) : value(0.0), entity(entity), connectedNeuron(nullptr) {}

void Sensor::clearSelectedChunks() {
    this->selectedChunks.clear();
}

const std::vector<Point> &Sensor::getSelectedChunks() const {
    return selectedChunks;
}

void Sensor::setSelectedChunks(const std::vector<Point> &selectedChunks) {
    Sensor::selectedChunks = selectedChunks;
}

void Sensor::passValueToNeuron() {
    this->connectedNeuron->setValue(this->value);
}

void Sensor::setConnectedNeuron(InputNeuron *connectedNeuron) {
    Sensor::connectedNeuron = connectedNeuron;
}

void Sensor::setEntity(Body *entity) {
    Sensor::entity = entity;
}
