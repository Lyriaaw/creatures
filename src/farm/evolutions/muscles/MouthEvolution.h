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
    void generateFromRandom(BrainConnector * connector) override;
    void perform(BrainConnector * connector) override;
    std::string describe() override;
    Evolution * generateWithMate(Evolution * mate) override;
    Evolution * generateFromCastedMate(MouthEvolution * mate);

};


#endif //CREATURES_MOUTHEVOLUTION_H
