//
// Created by Amalric Lombard de Buffières on 8/9/20.
//

#include "Muscle.h"

double Muscle::getEnergyConsumption() const {
    return energyConsumption;
}
double Muscle::getAndClearEnergyConsumption() {
    double tmp = energyConsumption;

    this->energyConsumption = 0.0;

    return tmp;
}


void Muscle::setEnergyConsumption(double energyConsumption) {
    Muscle::energyConsumption = energyConsumption;
}

const std::vector<Point> &Muscle::getSelectedTiles() const {
    return selectedTiles;
}


void Muscle::clearSelectedTiles() {
    this->selectedTiles.clear();
}

void Muscle::findSelectedTiles() {
    this->selectedTiles.clear();
    float muscleRotation = (float(rotation) + entity->getRotation()) * float(M_PI);

    float muscleX = (cos(muscleRotation) * entity->getSize()) + entity->getPosition().getX();
    float muscleY = (sin(muscleRotation) * entity->getSize()) + entity->getPosition().getY();

    Point musclePoint = Point(muscleX, muscleY);
    Point tile = musclePoint.getTileCoordinates();


    for (int it = -1; it <= 1; it++) {
        for (int jt = -1; jt <= 1; jt++) {
            if (it < 0 || it >= TILE_COUNT_WIDTH || jt < 0 || jt >= TILE_COUNT_HEIGHT)
                continue;

            float x = tile.getX() + it;
            float y = tile.getY() + jt;

            if (x < 0)
                x = 0;
            if (y < 0)
                y = 0;
            if (x >= TILE_COUNT_WIDTH)
                x = TILE_COUNT_WIDTH - 1;
            if (y >= TILE_COUNT_HEIGHT)
                y = TILE_COUNT_HEIGHT - 1;

            selectedTiles.emplace_back(Point(x, y));
        }
    }


}

Muscle::Muscle(float rotation, Entity *entity) : rotation(rotation), entity(entity), energyConsumption(0) {}

const std::vector<OutputNeuron *> &Muscle::getNeurons() const {
    return neurons;
}

void Muscle::setNeurons(const std::vector<OutputNeuron *> &neurons) {
    Muscle::neurons = neurons;
}

void Muscle::addNeuron(OutputNeuron * neuron) {
    neurons.emplace_back(neuron);
}


void Muscle::setEntity(Entity *entity) {
    Muscle::entity = entity;
}

float Muscle::getRotation() const {
    return rotation;
}

double Muscle::getValue() {
    return this->neurons.at(0)->getValue();
}
