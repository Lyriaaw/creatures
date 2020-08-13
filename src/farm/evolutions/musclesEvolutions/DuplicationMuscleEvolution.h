
//
// Created by Amalric Lombard de Buffières on 8/12/20.
//

#ifndef CREATURES_DUPLICATIONMUSCLEEVOLUTION_H
#define CREATURES_DUPLICATIONMUSCLEEVOLUTION_H


#include "../Evolution.h"

class DuplicationMuscleEvolution: public Evolution {
private:
    float rotation;
public:
    DuplicationMuscleEvolution();
    void generateFromRandom(Life * life) override;
    void perform(Life * life) override;
    std::string describe() override;
    Evolution * generateWithMate(Evolution * mate) override;
};


#endif //CREATURES_DUPLICATIONMUSCLEEVOLUTION_H
