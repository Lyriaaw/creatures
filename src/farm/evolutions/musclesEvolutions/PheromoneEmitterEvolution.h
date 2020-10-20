//
// Created by Amalric Lombard de Buffières on 10/20/20.
//

#ifndef CREATURES_PHEROMONEEMITTEREVOLUTION_H
#define CREATURES_PHEROMONEEMITTEREVOLUTION_H


#include "../Evolution.h"

class PheromoneEmitterEvolution: public Evolution {
private:
    double color;
public:
    PheromoneEmitterEvolution();
    void generateFromRandom(Life * life) override;
    void perform(Life * life) override;
    std::string describe() override;
    Evolution * generateWithMate(Evolution * mate) override;
    Evolution * generateFromCastedMate(PheromoneEmitterEvolution * mate);
    std::string getName() override;

};


#endif //CREATURES_PHEROMONEEMITTEREVOLUTION_H
