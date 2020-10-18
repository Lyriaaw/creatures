//
// Created by Amalric Lombard de Buffières on 10/18/20.
//

#ifndef CREATURES_NEURONEVOLUTION_H
#define CREATURES_NEURONEVOLUTION_H


#include "../Evolution.h"

class NeuronEvolution: public Evolution {
private:
    Neuron * neuron;

    float x, y;
public:
    NeuronEvolution();
    void generateFromRandom(Life * life) override;
    void perform(Life * life) override;
    std::string describe() override;
    Evolution * generateWithMate(Evolution * mate) override;
    Evolution * generateFromCastedMate(NeuronEvolution * mate);
    std::string getName() override;

    void generateFromXandY(float neuronX, float neuronY);

    Neuron *getNeuron() const;

    float getX() const;

    float getY() const;
};


#endif //CREATURES_NEURONEVOLUTION_H
