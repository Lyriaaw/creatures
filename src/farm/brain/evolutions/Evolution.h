//
// Created by Amalric Lombard de Buffières on 8/2/20.
//

#ifndef CREATURES_EVOLUTION_H
#define CREATURES_EVOLUTION_H


#include "../../Creature.h"
#include "../BrainConnector.h"

class Evolution {
protected:
    int generationNumber;

public:
    static int nextEvolutionNumber;

    Evolution();

    virtual void generateFromRandom(BrainConnector * connector) = 0;

    virtual void perform(BrainConnector * connector) = 0;

    virtual void describe() = 0;

    int getGenerationNumber();

    void setGenerationNumber(int generationNumber);
    void generateGenerationNumber();
};




#endif //CREATURES_EVOLUTION_H
