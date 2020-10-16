//
// Created by Amalric Lombard de Buffières on 10/16/20.
//

#ifndef CREATURES_MEMORYEVOLUTION_H
#define CREATURES_MEMORYEVOLUTION_H


#include "../Evolution.h"

class MemoryEvolution: public Evolution {
private:
    int dataCount;
public:
    MemoryEvolution();
    void generateFromRandom(Life * life) override;
    void perform(Life * life) override;
    std::string describe() override;
    Evolution * generateWithMate(Evolution * mate) override;
};


#endif //CREATURES_MEMORYEVOLUTION_H
