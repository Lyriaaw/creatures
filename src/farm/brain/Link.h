//
// Created by Amalric Lombard de Buffières on 8/3/20.
//

#ifndef CREATURES_LINK_H
#define CREATURES_LINK_H


#include "neurons/Neuron.h"

class Link {
private:
    float weight;

    Neuron * input;
    Neuron * output;

public:
    Link(float weight, Neuron *input, Neuron *output);

    float getWeight() const;

    void setWeight(float weight);

    Neuron *getInput() const;

    void setInput(Neuron *input);

    Neuron *getOutput() const;

    void setOutput(Neuron *output);
};


#endif //CREATURES_LINK_H
