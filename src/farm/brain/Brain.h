//
// Created by Amalric Lombard de Buffières on 8/2/20.
//

#ifndef CREATURES_BRAIN_H
#define CREATURES_BRAIN_H


#include <vector>
#include "neurons/Neuron.h"

class Brain {

public:
    const std::vector<Neuron *> &getNeurons() const;

    void setNeurons(const std::vector<Neuron *> &neurons);
    void addNeuron(Neuron * neuron);

private:
    std::vector<Neuron *> neurons;
};


#endif //CREATURES_BRAIN_H
