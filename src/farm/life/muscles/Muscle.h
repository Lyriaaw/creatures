//
// Created by Amalric Lombard de Buffières on 8/9/20.
//

#ifndef CREATURES_MUSCLE_H
#define CREATURES_MUSCLE_H


#include "../../brain/neurons/OutputNeuron.h"

class Muscle {
protected:
    std::vector<OutputNeuron *> neurons;
    float rotation;
    Entity * entity;

    double energyConsumption;

    std::vector<Point> selectedTiles;


public:

    double getEnergyConsumption() const;

    void setEnergyConsumption(double energyConsumption);

    double getAndClearEnergyConsumption();

    const std::vector<Point> &getSelectedTiles() const;
    void findSelectedTiles();
    void clearSelectedTiles();

    Muscle(float rotation, Entity *entity);

    const std::vector<OutputNeuron *> &getNeurons() const;

    void setNeurons(const std::vector<OutputNeuron *> &neurons);
    void addNeuron(OutputNeuron * neuron);

    void setEntity(Entity *entity);

    float getRotation() const;

    virtual std::string getName() = 0;

    double getValue();
};


#endif //CREATURES_MUSCLE_H
