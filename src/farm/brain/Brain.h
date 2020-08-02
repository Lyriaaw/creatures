//
// Created by Amalric Lombard de Buffières on 8/2/20.
//

#ifndef CREATURES_BRAIN_H
#define CREATURES_BRAIN_H


#include <vector>
#include "neurons/InputNeuron.h"

class Brain {

public:
    const std::vector<Neuron *> &getNeurons() const;

    void setNeurons(const std::vector<Neuron *> &neurons);
    void addNeuron(Neuron * neuron);
    void addInputNeuron(InputNeuron * neuron);

    const std::vector<InputNeuron *> &getInputNeurons() const;


private:
    std::vector<Neuron *> neurons;
    std::vector<InputNeuron *> inputNeurons;
};


#endif //CREATURES_BRAIN_H
