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

const std::vector<Point> &Muscle::getSelectedChunks() const {
    return selectedChunks;
}

void Muscle::setSelectedChunks(const std::vector<Point> &selectedChunks) {
    Muscle::selectedChunks = selectedChunks;
}

void Muscle::clearSelectedChunks() {
    this->selectedChunks.clear();
}

void Muscle::findSelectedChunks() {
    float muscleRotation = (float(rotation) + entity->getRotation()) * float(M_PI);

    float muscleX = (cos(muscleRotation) * entity->getSize()) + entity->getPosition().getX();
    float muscleY = (sin(muscleRotation) * entity->getSize()) + entity->getPosition().getY();

    Point musclePoint = Point(muscleX, muscleY);
    Point tile = musclePoint.getTileCoordinates();

    if (tile.getX() < 0)
        tile.setX(0);
    if (tile.getY() < 0)
        tile.setY(0);
    if (tile.getX() > TILE_COUNT_WIDTH)
        tile.setX(TILE_COUNT_WIDTH - 1);
    if (tile.getY() > TILE_COUNT_HEIGHT)
        tile.setY(TILE_COUNT_HEIGHT - 1);

    for (int it = -1; it <= 1; it++) {
        for (int jt = -1; jt <= 1; jt++) {
            if (it < 0 || it >= TILE_COUNT_WIDTH || jt < 0 || jt >= TILE_COUNT_HEIGHT)
                continue;
            selectedChunks.emplace_back(Point(tile.getX() + it, tile.getY() + jt));
        }
    }


}

Muscle::Muscle(float rotation, Entity *entity) : rotation(rotation), entity(entity) {}

const std::vector<OutputNeuron *> &Muscle::getNeurons() const {
    return neurons;
}

void Muscle::setNeurons(const std::vector<OutputNeuron *> &neurons) {
    Muscle::neurons = neurons;
}

void Muscle::addNeuron(OutputNeuron * neuron) {
    neurons.emplace_back(neuron);
}


