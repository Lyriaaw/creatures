//
// Created by Amalric Lombard de Buffières on 8/3/20.
//

#ifndef CREATURES_MOUTHEVOLUTION_H
#define CREATURES_MOUTHEVOLUTION_H


#include "../Evolution.h"

class MouthEvolution: public Evolution {
private:
    float rotation;
public:
    MouthEvolution();
    void generateFromRandom(Life * life) override;
    void perform(Life * life) override;
    std::string describe() override;
    Evolution * generateWithMate(Evolution * mate) override;
    Evolution * generateFromCastedMate(MouthEvolution * mate);
    std::string getName() override;
};


#endif //CREATURES_MOUTHEVOLUTION_H
