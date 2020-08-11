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

    std::vector<Point> selectedChunks;


public:

    double getEnergyConsumption() const;

    void setEnergyConsumption(double energyConsumption);

    double getAndClearEnergyConsumption();

    const std::vector<Point> &getSelectedChunks() const;

    void setSelectedChunks(const std::vector<Point> &selectedChunks);

    void findSelectedChunks();
    void clearSelectedChunks();

    Muscle(float rotation, Entity *entity);

    const std::vector<OutputNeuron *> &getNeurons() const;

    void setNeurons(const std::vector<OutputNeuron *> &neurons);
    void addNeuron(OutputNeuron * neuron);

    void setEntity(Entity *entity);
};


#endif //CREATURES_MUSCLE_H
